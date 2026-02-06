
#pragma once
#include <android/log.h>
#include "../include/Utils/Unity/ByNameModding/Tools.h"
#include "../include/Utils/Unity/ByNameModding/Il2Cpp.h"

namespace Unrelease {
    extern bool g_Enabled;

    // ==========================================
    // TIPE 1: Untuk fungsi dengan 1 Argumen C# (Total 2: Instance + Arg1)
    // Contoh: IsForbidHeros(int heroId)
    // ==========================================
    #define DEFINE_HOOK_ARGS_1(FUNC_NAME) \
        bool (*o##FUNC_NAME)(void* instance, void* arg1); \
        bool h##FUNC_NAME(void* instance, void* arg1) { \
            if (g_Enabled) { \
                static bool logged = false; \
                if (!logged) { \
                    __android_log_print(ANDROID_LOG_INFO, "StealthMod", "Active: %s (Arg: 1)", #FUNC_NAME); \
                    logged = true; \
                } \
                return false; \
            } \
            return o##FUNC_NAME(instance, arg1); \
        }

    // ==========================================
    // TIPE 2: Khusus IsForbidSkin dengan 2 Argumen C# (Total 3: Instance + Arg1 + Arg2)
    // Contoh: IsForbidSkin(int skinId, bool isLocal)
    // ==========================================
    #define DEFINE_HOOK_ARGS_2(FUNC_NAME) \
        bool (*o##FUNC_NAME)(void* instance, void* arg1, void* arg2); \
        bool h##FUNC_NAME(void* instance, void* arg1, void* arg2) { \
            if (g_Enabled) { \
                static bool logged = false; \
                if (!logged) { \
                     __android_log_print(ANDROID_LOG_INFO, "StealthMod", "Active: %s (Arg: 2)", #FUNC_NAME); \
                    logged = true; \
                } \
                return false; \
            } \
            return o##FUNC_NAME(instance, arg1, arg2); \
        }

    // --- Implementasi Hook ---

    // 1. Khusus yang argumennya 2
    DEFINE_HOOK_ARGS_2(IsForbidSkin) 

    // 2. Yang argumennya 1 (Standar)
    DEFINE_HOOK_ARGS_1(IsForbidARSkin)
    DEFINE_HOOK_ARGS_1(IsForbidHeros)
    DEFINE_HOOK_ARGS_1(IsForbidARHeros)
    DEFINE_HOOK_ARGS_1(IsForbidHeroInChooseHero)
    DEFINE_HOOK_ARGS_1(IsActivityForbidHeros)
    DEFINE_HOOK_ARGS_1(IsReplaceForbidHeros)
    DEFINE_HOOK_ARGS_1(IsForbidNewHeroList)
    DEFINE_HOOK_ARGS_1(IsForbidHeadFrameForce)
    DEFINE_HOOK_ARGS_1(IsForbidHeroDisOrder)
    DEFINE_HOOK_ARGS_1(IsForbidSkinNumTag)
    DEFINE_HOOK_ARGS_1(IsForbidHero1v1)
    DEFINE_HOOK_ARGS_1(IsForbidHeadFrame)
    DEFINE_HOOK_ARGS_1(IsForbidNameSkin)
    DEFINE_HOOK_ARGS_1(IsForbidNameColor)
    DEFINE_HOOK_ARGS_1(IsForbidRoomBorder)
    DEFINE_HOOK_ARGS_1(IsForbidDragonCrystal)
    DEFINE_HOOK_ARGS_1(IsForbidStatue)

    // --- Inisialisasi ---
    inline void Init() {
        if (!g_Enabled) return;

        // Macro Helper Lokal untuk Init agar rapi
        // ARGS: Jumlah argumen C# (dipakai untuk mencari method offset)
        #define REGISTER_HOOK(METHOD, ARGS, FUNC_NAME) \
            Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", METHOD, ARGS), (void*)h##FUNC_NAME, (void**)&o##FUNC_NAME)

        // 1. Register IsForbidSkin (Args: 2)
        REGISTER_HOOK("IsForbidSkin", 2, IsForbidSkin);

        // 2. Register Sisanya (Args: 1)
        REGISTER_HOOK("IsForbidHeros", 1, IsForbidHeros);
        REGISTER_HOOK("IsForbidARSkin", 1, IsForbidARSkin);
        REGISTER_HOOK("IsForbidARHeros", 1, IsForbidARHeros);
        REGISTER_HOOK("IsForbidHeroInChooseHero", 1, IsForbidHeroInChooseHero);
        REGISTER_HOOK("IsActivityForbidHeros", 1, IsActivityForbidHeros);
        REGISTER_HOOK("IsReplaceForbidHeros", 1, IsReplaceForbidHeros);
        REGISTER_HOOK("IsForbidNewHeroList", 1, IsForbidNewHeroList);
        REGISTER_HOOK("IsForbidHeadFrameForce", 1, IsForbidHeadFrameForce);
        REGISTER_HOOK("IsForbidHeroDisOrder", 1, IsForbidHeroDisOrder);
        REGISTER_HOOK("IsForbidSkinNumTag", 1, IsForbidSkinNumTag);
        REGISTER_HOOK("IsForbidHero1v1", 1, IsForbidHero1v1);
        REGISTER_HOOK("IsForbidHeadFrame", 1, IsForbidHeadFrame);
        REGISTER_HOOK("IsForbidNameSkin", 1, IsForbidNameSkin);
        REGISTER_HOOK("IsForbidNameColor", 1, IsForbidNameColor);
        REGISTER_HOOK("IsForbidRoomBorder", 1, IsForbidRoomBorder);
        REGISTER_HOOK("IsForbidDragonCrystal", 1, IsForbidDragonCrystal);
        REGISTER_HOOK("IsForbidStatue", 1, IsForbidStatue);
    }
}

