#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"
#include <sys/stat.h>

#ifdef WIN32
bool is_windows = true;
#else
bool is_windows = false;
#endif

void append_all(Nob_Cmd* cmd, const char** args, size_t count) {
    for (size_t i = 0; i < count; i++) { //NOLINT
        da_append(cmd, args[i]);
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

void get_raylib() {
    if (!dir_exists("./raylib-5.5/")) {
        Cmd download = { 0 };
        Cmd unzip = { 0 };
        Cmd move = { 0 };

        cmd_append(&download, "curl");
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
        if (!cmd_run_sync(download)) nob_log(ERROR, "Download failed..."); return;
        if (!cmd_run_sync(unzip))    nob_log(ERROR, "Unzip failed...");    return;
        if (!cmd_run_sync(move))     nob_log(ERROR, "Move failed...");     return;

        cmd_free(download);
        cmd_free(unzip);
        cmd_free(move);
    }
}

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    get_raylib();

    const char* build = "build/";
    const char* exe = is_windows ? "Building-a-Collection.exe" : "Building-a-Collection";
    const char* raylib = is_windows ? "./raylib-5.5/lib/raylib.lib" : "./raylib-5.5/lib/libraylib.a";
    const char* windows_libs[] = {
        "-lmsvcrt",
        "-lshell32",
        "-lgdi32",
        "-lwinmm",
        "-lopengl32",
        "-lkernel32",
        "-luser32",
    };
    const char* warnings[] = {
        "-Wno-c99-designator",
    };
    const char* raylib_include = "-I./raylib-5.5/include/";
    const char* os_flags = is_windows ? "-nostdlib" : "";
    const char* flags[] = { "-g", "-O0", "-std=c++20" };

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
    cmd_append(&cmd, "-c");
    nob_cc_inputs(&cmd, "./main.cpp");
    nob_cc_output(&cmd, nob_temp_sprintf("%smain.o", build));
    cmd_append(&cmd, raylib_include);
    append_all(&cmd, warnings, ARRAY_LEN(warnings));
    append_all(&cmd, flags, ARRAY_LEN(flags));
    Procs procs = {0};
    da_append(&procs, cmd_run_async(cmd));
    for (size_t i = 0; i < src.count; i++) {
        cmd.count = 0;
        cmd_append(&cmd, cxx);
        cmd_append(&cmd, "-c");
        if (!sv_end_with(sv_from_cstr(src.items[i]), ".cpp")) { continue; }
        cmd_append(&cmd, nob_temp_sprintf("src/%s", src.items[i]));
        const char* filename = nob_temp_sprintf("%.*s", (int) (strlen(src.items[i]) - 4), src.items[i]);
        nob_cc_output(&cmd, nob_temp_sprintf("%s%s.o", build, filename));
        cmd_append(&cmd, raylib_include);
        append_all(&cmd, warnings, ARRAY_LEN(warnings));
        append_all(&cmd, flags, ARRAY_LEN(flags));
        da_append(&procs, cmd_run_async(cmd));
    }

    procs_wait(procs);
    cmd.count = 0;
    cmd_append(&cmd, cxx);
    da_free(src);
    da_free(procs);
    File_Paths obj = {0};
    if (!read_entire_dir(build, &obj)) return -1;
    for (size_t i = 0; i < obj.count; i++) {
        if (!sv_end_with(sv_from_cstr(obj.items[i]), ".o")) { continue; }
        cmd_append(&cmd, nob_temp_sprintf("%s%s", build, obj.items[i]));
    }
    nob_cc_output(&cmd, exe);

    cmd_append(&cmd, raylib_include);
    cmd_append(&cmd, raylib);
    cmd_append(&cmd, os_flags);
    append_all(&cmd, flags, ARRAY_LEN(flags));
    append_all(&cmd, warnings, ARRAY_LEN(warnings));


    da_free(obj);

    if (is_windows) {
        append_all(&cmd, windows_libs, ARRAY_LEN(windows_libs));
    }

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
