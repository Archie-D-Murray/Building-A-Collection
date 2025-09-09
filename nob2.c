#define NOB_IMPLEMENTATION
#include "nob.h"

#ifdef WIN32
bool is_windows = true;
#else
bool is_windows = false;
#endif

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    const char* exe = is_windows ? "test.exe" : "test";
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
    const char* os_flags = is_windows ? "-nostdlib" : "";

    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "clang++");
    nob_cc_inputs(&cmd, "./test.cpp", "./src/utilities.cpp");
    nob_cc_output(&cmd, exe);
    nob_da_append(&cmd, "-I./raylib-5.5/include/");
    nob_da_append(&cmd, raylib);
    nob_da_append(&cmd, os_flags);

    for (size_t i = 0; i < NOB_ARRAY_LEN(windows_libs) && is_windows; i++) {
        nob_da_append(&cmd, windows_libs[i]);
    }

    if (!nob_cmd_run_sync_and_reset(&cmd)) {
        nob_log(NOB_ERROR, "Compile failed...");
        return -1;
    }

    nob_da_append(&cmd, exe);
    if (!nob_cmd_run_sync_and_reset(&cmd)) {
        nob_log(NOB_ERROR, "Failed to run executable...");
        return -1;
    }
}
