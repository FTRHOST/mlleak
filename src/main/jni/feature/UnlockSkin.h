#pragma once
#include <vector>
#include <set>

namespace UnlockSkin {
    // --- Global Config ---
    extern bool g_Enabled;
    uint32_t m_HeroID = 0;
    uint32_t m_SkinID = 0;

    // --- Structs (Unity Layout) ---
    struct CmdHeroSkin : Il2CppObject {
        uint32_t iId, iLimitTime, iSource, iLimitTimeMagicChess, iGetTime;
    };

    struct CmdHeroStatue : Il2CppObject {
        uint32_t iId, iLimitTime, iSource;
    };

    // --- Original Function Pointers ---
    void* (*oGetHeroSkin)(void*, uintptr_t, uint32_t);
    void* (*oIsHaveSkin)(void*, uint32_t);
    void* (*oIsHaveSkinForever)(void*, uint32_t);
    void (*oSendRawData)(void*, uint32_t, void*, int, int, int, bool, int);

    // ==========================================
    // TIPE 1: Untuk fungsi dengan 1 Argumen C# (Total 2: Instance + Arg1)
    // Contoh: IsForbidHeros(int heroid)
    // ==========================================
    #define DEFINE_HOOK_TRUE_ARGS_1(FUNC_NAME) \
        bool (*o##FUNC_NAME)(void* unk, void* arg1); \
        bool h##FUNC_NAME(void* unk, void* arg1) { \
            if (g_Enabled) { \
                static bool logged = false; \
                if (!logged) { \
                    __android_log_print(ANDROID_LOG_INFO, "StealthMod", "Active: %s (Arg: 1)", #FUNC_NAME); \
                    logged = true; \
                } \
                return true; \
            } \
            return o##FUNC_NAME(unk, arg1); \
        }

    DEFINE_HOOK_TRUE_ARGS_1(IsCanUseSkin)
    DEFINE_HOOK_TRUE_ARGS_1(GetLeaderSkinBForbid)
    DEFINE_HOOK_TRUE_ARGS_1(BRankHeroCanUse)
    DEFINE_HOOK_TRUE_ARGS_1(CheckReputationUnlockSkin)
    DEFINE_HOOK_TRUE_ARGS_1(IsLimitActiveHero)
    DEFINE_HOOK_TRUE_ARGS_1(IsHeroInShop)


    // --- Hook Implementations ---
    void* hGetHeroSkin(void* unk, uintptr_t m_heroskins, uint32_t skinid) {
        void* ret = oGetHeroSkin(unk, m_heroskins, skinid);
        if (ret || !g_Enabled) return ret;

        // Create fake instance if not owned
        auto newInst = (CmdHeroSkin*)Il2CppCreateClassInstance("Assembly-CSharp.dll", "MTTDProto", "CmdHeroSkin");
        if (newInst) {
            newInst->iId = skinid;
            newInst->iLimitTime = 0;
            newInst->iSource = 0;
            newInst->iLimitTimeMagicChess = 0;
            newInst->iGetTime = 0;
        }
        return newInst;
    }

    void* hIsHaveSkin(void* unk, uint32_t skinid) {
        void* ret = oIsHaveSkin(unk, skinid);
        if (ret || !g_Enabled) return ret;

        // Create fake instance if not owned
        auto newInst = (CmdHeroSkin*)Il2CppCreateClassInstance("Assembly-CSharp.dll", "MTTDProto", "CmdHeroSkin");
        if (newInst) {
            newInst->iId = skinid;
            newInst->iLimitTime = 0;
            newInst->iSource = 0;
            newInst->iLimitTimeMagicChess = 0;
            newInst->iGetTime = 0;
        }
        return newInst;
    }

    void* hIsHaveSkinForever(void* unk, uint32_t skinid) {
        void* ret = oIsHaveSkinForever(unk, skinid);
        if (ret || !g_Enabled) return ret;

        // Create fake instance if not owned
        auto newInst = (CmdHeroSkin*)Il2CppCreateClassInstance("Assembly-CSharp.dll", "MTTDProto", "CmdHeroSkin");
        if (newInst) {
            newInst->iId = skinid;
            newInst->iLimitTime = 0;
            newInst->iSource = 0;
            newInst->iLimitTimeMagicChess = 0;
            newInst->iGetTime = 0;
        }
        return newInst;
    }


    // Protection against sending skin-related data to server (Antiban)
    void hSendRawData(void* instance, uint32_t msgId, void* data, int size, int socketType, int packType, bool lock, int expSize) {
        static const std::set<uint32_t> blockedMsgs = {
            1015, 1016, 1019, 1020, 1031, 1032, 1035, 1036, 1160, 1161, 1162, 1163,
            1208, 10017, 10018, 10192, 10193, 10603, 10604, 19457, 19458, 19459, 19460
        };

        if (g_Enabled && blockedMsgs.count(msgId)) return;
        oSendRawData(instance, msgId, data, size, socketType, packType, lock, expSize);
    }

    // --- Initialization ---
    inline void Init() {
        if (!g_Enabled) return;

        void* addrGetSkin = Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", "GetHeroSkin", 2);
        if (addrGetSkin) Tools::Hook(addrGetSkin, (void*)hGetHeroSkin, (void**)&oGetHeroSkin);

        void* addrIsHaveSkin = Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", "IsHaveSkin", 1);
        if(addrIsHaveSkin) Tools::Hook(addrIsHaveSkin, (void*)hIsHaveSkin, (void**)&oIsHaveSkin);
        
        void* addrIsHaveSkinForever = Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", "IsHaveSkinForever", 1);
        if(addrIsHaveSkinForever) Tools::Hook(addrIsHaveSkinForever, (void*)hIsHaveSkinForever, (void**)&oIsHaveSkinForever);

        void* addrIsCanUse = Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", "IsCanUseSkin", 1);
        if(addrIsCanUse) Tools::Hook(addrIsCanUse, (void*)hIsCanUseSkin, (void**)&oIsCanUseSkin);

        void* addrBRankHeroCanUse = Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "UIRankHero", "BRankHeroCanUse", 1);
        if(addrBRankHeroCanUse) Tools::Hook(addrBRankHeroCanUse, (void*)hBRankHeroCanUse, (void**)&oBRankHeroCanUse);

        void* addrCheckReputationUnlockSkin = Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", "CheckReputationUnlockSkin", 1);
        if(addrCheckReputationUnlockSkin) Tools::Hook(addrCheckReputationUnlockSkin, (void*)hCheckReputationUnlockSkin, (void**)&oCheckReputationUnlockSkin);

        void* addrIsLimitActiveHero = Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", "IsLimitActiveHero", 1);
        if(addrIsLimitActiveHero) Tools::Hook(addrIsLimitActiveHero, (void*)hIsLimitActiveHero, (void**)&oIsLimitActiveHero);

        void* addrIsHeroInShop = Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", "IsHeroInShop", 1);
        if(addrIsHeroInShop) Tools::Hook(addrIsHeroInShop, (void*)hIsHeroInShop, (void**)&oIsHeroInShop);

        void* addrSendData = Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "GameServerConfig", "SendRawData", 7);
        if (addrSendData) Tools::Hook(addrSendData, (void*)hSendRawData, (void**)&oSendRawData);
    }
}
