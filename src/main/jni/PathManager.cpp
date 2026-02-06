#include "PathManager.h"
#include <fstream>
#include <unistd.h>
#include <android/log.h>
#include <sys/stat.h>
#include <sys/types.h>

#define LOG_TAG "StealthMod"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Helper function to get the current process's base package name
static std::string getPackageName() {
    static std::string cached_pkg;
    if (!cached_pkg.empty()) return cached_pkg;

    char buffer[1024] = {0};
    FILE* cmdline = fopen("/proc/self/cmdline", "r");
    if (cmdline) {
        fread(buffer, sizeof(buffer), 1, cmdline);
        fclose(cmdline);
        
        std::string full_cmd(buffer);
        size_t colon_pos = full_cmd.find(':');
        if (colon_pos != std::string::npos) {
            cached_pkg = full_cmd.substr(0, colon_pos);
        } else if (!full_cmd.empty()) {
            cached_pkg = full_cmd;
        }
    }
    if (cached_pkg.empty()) {
        __android_log_print(ANDROID_LOG_ERROR, "StealthMod", "Failed to get package name from /proc/self/cmdline");
    } else {
        __android_log_print(ANDROID_LOG_INFO, "StealthMod", "Detected Package Name: %s", cached_pkg.c_str());
    }
    return cached_pkg;
}

std::string getDynamicFilesDir() {
    static std::string files_dir_cache; 
    if (!files_dir_cache.empty()) return files_dir_cache;

    std::string pkgName = getPackageName();
    if (pkgName.empty()) return "";

    // 1. Coba External Path (User Friendly)
    std::string external = "/storage/emulated/0/Android/data/" + pkgName + "/files";
    mkdir(("/storage/emulated/0/Android/data/" + pkgName).c_str(), 0777);
    if (mkdir(external.c_str(), 0777) == 0 || errno == EEXIST) {
        // Cek apakah bisa menulis ke path ini
        std::string testFile = external + "/.test";
        std::ofstream ofs(testFile);
        if (ofs.is_open()) {
            ofs.close();
            remove(testFile.c_str());
            files_dir_cache = external;
            LOGI("Using External Path: %s", files_dir_cache.c_str());
            return files_dir_cache;
        }
    }

    // 2. Fallback ke Internal Path (Selalu Berhasil)
    files_dir_cache = "/data/data/" + pkgName + "/files";
    mkdir(files_dir_cache.c_str(), 0777);
    LOGI("Using Internal Fallback Path: %s", files_dir_cache.c_str());
    
    return files_dir_cache;
}

std::string getDynamicConfigPath() {
    std::string filesDir = getDynamicFilesDir();
    if (!filesDir.empty()) {
        return filesDir + "/config.json";
    }
    return ""; 
}