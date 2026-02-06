#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <android/log.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <sys/stat.h>

// Include JSON Library
#include "include/nlohmann/json.hpp"

// Include Modding Utils
#include "include/Utils/Unity/ByNameModding/Il2Cpp.h"
#include "include/Utils/Unity/ByNameModding/Tools.h"

// Include Project Headers
#include "PathManager.h"
#include "feature/UnlockSkin.h"
#include "feature/Template.h"
#include "feature/Unrelease.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "StealthMod"

#ifdef LOGI
#undef LOGI
#endif
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#ifdef LOGE
#undef LOGE
#endif
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

using json = nlohmann::json;

// --- Global State ---
namespace UnlockSkin { bool g_Enabled = false; }
namespace FiturBaru { bool g_Enabled = false; }
namespace Unrelease { bool g_Enabled = false; }

bool g_DebugMode = true;

// --- Config Management ---
void ApplyDefaultConfig(const std::string& path) {
    LOGI("Creating default config at: %s", path.c_str());
    std::ofstream outFile(path);
    if (outFile.is_open()) {
        json j = {
            {"UnlockCustomSkin", true},
            {"DebugMode", true},
            {"Unrelease", true},
            {"EnableTemplate", false}
        };
        outFile << j.dump(4);
        outFile.close();
        
        UnlockSkin::g_Enabled = true;
        g_DebugMode = true;
        Unrelease::g_Enabled = true;
        FiturBaru::g_Enabled = false;
        LOGI("Default config written successfully.");
    } else {
        LOGE("FAILED to create config file at %s. Check folder permissions!", path.c_str());
    }
}

void ReadConfig() {
    static time_t last_modified = 0;
    std::string path = getDynamicConfigPath();
    if (path.empty()) return;

    struct stat st;
    if (stat(path.c_str(), &st) != 0) {
        ApplyDefaultConfig(path);
        return;
    }

    if (st.st_mtime <= last_modified) return;
    last_modified = st.st_mtime;

    LOGI("Config file change detected. Reloading...");
    std::ifstream configFile(path);
    if (configFile.is_open()) {
        try {
            json j;
            configFile >> j;

            if (j.contains("UnlockCustomSkin")) UnlockSkin::g_Enabled = j["UnlockCustomSkin"].get<bool>();
            if (j.contains("DebugMode")) g_DebugMode = j["DebugMode"].get<bool>();
            if (j.contains("EnableTemplate")) FiturBaru::g_Enabled = j["EnableTemplate"].get<bool>();
            if (j.contains("Unrelease")) Unrelease::g_Enabled = j["Unrelease"].get<bool>();

            if (g_DebugMode) {
                LOGI("Config Updated -> UnlockAll:%d, Unrelease:%d, Template:%d", 
                     UnlockSkin::g_Enabled, Unrelease::g_Enabled, FiturBaru::g_Enabled);
            }
        } catch (const json::parse_error& e) {
            LOGE("JSON Parse Error: %s", e.what());
        }
    }
}

// --- Mod Lifecycle ---
void ApplyFeatures() {
    if (!Il2CppIsAssembliesLoaded()) return;

    static bool initialized = false;
    if (!initialized) {
        LOGI("Initializing feature hooks...");
        UnlockSkin::Init();
        FiturBaru::Init();
        Unrelease::Init();
        initialized = true;
        LOGI("All hooks applied successfully.");
    }
}

void *StealthThread(void *) {
    LOGI("Stealth Service Thread Started.");
    ReadConfig();

    int retry = 0;
    while (!Il2CppIsAssembliesLoaded() && retry < 120) {
        Il2CppAttach();
        if (retry % 5 == 0) {
            LOGI("Waiting for Il2Cpp... (Attempt %d/120)", retry);
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
        retry++;
    }

    if (Il2CppIsAssembliesLoaded()) {
        LOGI("Il2Cpp detected! Applying initial features.");
        ApplyFeatures();

        while (true) {
            ReadConfig();
            ApplyFeatures();
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    } else {
        LOGE("Il2Cpp NOT detected after 240 seconds. Thread exiting.");
    }
    
    return nullptr;
}

// --- Entry Point ---
__attribute__((constructor))
void Initializer() {
    __android_log_print(ANDROID_LOG_INFO, "StealthMod", "Library Loaded! Initializing thread...");
    pthread_t pt;
    pthread_create(&pt, nullptr, StealthThread, nullptr);
}

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    return JNI_VERSION_1_6;
}
