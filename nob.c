#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"
#include <sys/stat.h>

#ifdef WIN32
bool is_windows = true;
#else
bool is_windows = false;
#endif

#if defined(WIN32) && !defined(__clang__)
bool msvc_only = true;
#else
bool msvc_only = false;
#endif

typedef struct {
    const char** items;
    size_t count;
    size_t capacity;
} String_Array;

void append_all(Nob_Cmd* cmd, String_Array* sb) {
    for (size_t i = 0; i < sb->count; i++) {
        da_append(cmd, sb->items[i]);
    }
}

void pop_many(Nob_Cmd* cmd, size_t count) {
    for (size_t i = 0; i < count; i++) {
        nob_da_remove_unordered(cmd, cmd->count - 1);
    }
}

bool dir_exists(const char* dir) {
    struct stat s;
    return stat(dir, &s) == 0 && (S_IFDIR & s.st_mode);
}

void sleep(size_t ms) {
    #ifdef WIN32
    Sleep(ms);
    #else
    sleep(ms);
    #endif
}

void get_raylib() {
    Cmd download = { 0 };
    Cmd unzip = { 0 };
    Cmd move = { 0 };

    cmd_append(&download, "curl", "-JL");

    if (is_windows) {
        cmd_append(&unzip, "tar");
        cmd_append(&download, "-o", "raylib-5.5.zip");
        cmd_append(&download, "https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_win64_msvc16.zip");
        cmd_append(&unzip, "-xf", "raylib-5.5.zip");
        cmd_append(&move, "ren", "raylib-5.5_win64_msvc16", "raylib-5.5");
    } else {
        cmd_append(&download, "-o", "raylib-5.5.tar.gz");
        cmd_append(&download, "https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_linux_amd64.tar.gz");
        cmd_append(&unzip, "-xzf", "raylib-5.5.tar.gz");
        cmd_append(&move, "mv", "raylib-5.5_linux_amd64", "raylib-5.5");
    }

    if (!cmd_run_sync(download)) { nob_log(ERROR, "Download failed..."); return; }
    sleep(250); // May need to wait on fs to sort out download
    if (!cmd_run_sync(unzip))    { nob_log(ERROR, "Unzip failed...");    return; }
    sleep(250); // May need to wait on fs to sort out unzip
    if (!cmd_run_sync(move))     { nob_log(ERROR, "Move failed...");     return; }

    cmd_free(download);
    cmd_free(unzip);
    cmd_free(move);
}

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!dir_exists("./raylib-5.5/")) {
        get_raylib();
    }

    const char* build = "build/";
    const char* exe = is_windows ? "Building-a-Collection.exe" : "Building-a-Collection";
    const char* raylib = is_windows ? "./raylib-5.5/lib/raylib.lib" : "./raylib-5.5/lib/libraylib.a";
    const char* no_link = msvc_only ? "/c" : "-c";
    String_Array windows_libs = { 0 };
    da_append(&windows_libs, msvc_only ? "msvcrt.lib" : "-lmsvcrt");
    da_append(&windows_libs, msvc_only ? "shell32.lib" : "-lshell32");
    da_append(&windows_libs, msvc_only ? "gdi32.lib" : "-lgdi32");
    da_append(&windows_libs, msvc_only ? "winmm.lib" : "-lwinmm");
    da_append(&windows_libs, msvc_only ? "opengl32.lib" : "-lopengl32");
    da_append(&windows_libs, msvc_only ? "kernel32.lib" : "-lkernel32");
    da_append(&windows_libs, msvc_only ? "user32.lib" : "-luser32");

    String_Array warnings = { 0 };
    #if defined(__clang__)
        da_append(&warnings, "-Wno-c99-designator");
    #endif

    String_Array raylib_include = { 0 };
    if (msvc_only) {
        da_append(&raylib_include, "/I");
        da_append(&raylib_include, "raylib-5.5\\include\\");
    } else {
        da_append(&raylib_include, "-I./raylib-5.5/include/");
    }

    String_Array os_flags = { 0 }; 
    if (is_windows) {
        if (msvc_only) {
            da_append(&os_flags, "/MD");
            da_append(&os_flags, "/FS");
        } else {
            da_append(&os_flags, "-nostdlib");
        }
    }
    String_Array flags = { 0 };
    da_append(&flags, msvc_only ? "/Z7" : "-g");
    da_append(&flags, msvc_only ? "/Od" : "-O0");
    da_append(&flags, msvc_only ? "/std:c++20" : "-std=c++20");

    if (!nob_mkdir_if_not_exists(build)) {
        nob_log(NOB_ERROR, "Could not create build directory...");
        return -1;
    }

    #ifdef __clang__ // Favour clang because it is more cross compatible
    const char* cxx = "clang++";
    #else 
    #ifdef WIN32
    const char* cxx = "cl.exe";
    #else
    const char* cxx = "g++";
    #endif
    #endif

    File_Paths src = {0};
    read_entire_dir("./src/", &src);
    Cmd cmd = {0};
    cmd_append(&cmd, cxx);
    if (msvc_only) cmd_append(&cmd, "/EHsc", "/nologo");
    cmd_append(&cmd, no_link);
    nob_cc_inputs(&cmd, "./main.cpp");
    if (msvc_only) {
        cmd_append(&cmd, nob_temp_sprintf("/Fo:%smain.o", build));
    } else {
        nob_cc_output(&cmd, nob_temp_sprintf("%smain.o", build));
    }
    append_all(&cmd, &raylib_include);
    append_all(&cmd, &warnings);
    append_all(&cmd, &flags);
    Procs procs = {0};
    da_append(&procs, cmd_run_async(cmd));
    for (size_t i = 0; i < src.count; i++) {
        cmd.count = 0;
        cmd_append(&cmd, cxx);
        if (msvc_only) cmd_append(&cmd, "/EHsc", "/nologo");
        cmd_append(&cmd, no_link);
        if (!sv_end_with(sv_from_cstr(src.items[i]), ".cpp")) { continue; }
        cmd_append(&cmd, nob_temp_sprintf("src/%s", src.items[i]));
        const char* filename = nob_temp_sprintf("%.*s", (int) (strlen(src.items[i]) - 4), src.items[i]);
        if (msvc_only) {
            cmd_append(&cmd, nob_temp_sprintf("/Fo:%s%s.o", build, filename)); // This is ridiculous
        } else {
            nob_cc_output(&cmd, nob_temp_sprintf("%s%s.o", build, filename));
        }
        append_all(&cmd, &raylib_include);
        append_all(&cmd, &warnings);
        append_all(&cmd, &flags);
        da_append(&procs, cmd_run_async(cmd));
    }

    procs_wait(procs);
    cmd.count = 0;
    cmd_append(&cmd, msvc_only ? "link" : cxx);
    da_free(src);
    da_free(procs);
    File_Paths obj = {0};
    if (!read_entire_dir(build, &obj)) return -1;
    for (size_t i = 0; i < obj.count; i++) {
        if (!sv_end_with(sv_from_cstr(obj.items[i]), ".o")) { continue; }
        cmd_append(&cmd, nob_temp_sprintf("%s%s", build, obj.items[i]));
    }
    nob_cc_output(&cmd, exe);

    if (!msvc_only) append_all(&cmd, &raylib_include);
    if (!msvc_only) cmd_append(&cmd, raylib);
    if (!msvc_only) append_all(&cmd, &warnings);
    if (!msvc_only) append_all(&cmd, &flags);
    if (!msvc_only) append_all(&cmd, &os_flags);


    da_free(obj);

    if (is_windows) {
        cmd_append(&cmd, raylib);
        append_all(&cmd, &windows_libs);
    }

    if (msvc_only) cmd_append(&cmd, "/NODEFAULTLIB:LIBCMT", "/nologo");

    if (!cmd_run_sync_and_reset(&cmd)) {
        nob_log(NOB_ERROR, "Compile failed...");
        return -1;
    }

    // cmd_append(&cmd, exe);
    // if (!cmd_run_sync_and_reset(&cmd)) {
    //     nob_log(NOB_ERROR, "Failed to run executable...");
    //     return -1;
    // }
}
