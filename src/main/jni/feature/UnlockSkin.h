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
    void (*oSendRawData)(void*, uint32_t, void*, int, int, int, bool, int);

    // --- Hook Implementations ---
    void* hGetHeroSkin(void* instance, uintptr_t list, uint32_t skinId) {
        void* ret = oGetHeroSkin(instance, list, skinId);
        if (ret || !g_Enabled) return ret;

        // Create fake instance if not owned
        auto newInst = (CmdHeroSkin*)Il2CppCreateClassInstance("Assembly-CSharp.dll", "MTTDProto", "CmdHeroSkin");
        if (newInst) {
            newInst->iId = skinId;
            newInst->iLimitTime = 0;
            newInst->iSource = 0;
            newInst->iLimitTimeMagicChess = 0;
            newInst->iGetTime = 0;
        }
        return newInst;
    }

      void* hIsHaveSkin(void* instance, uint32_t skinId) {
        void* ret = oIsHaveSkin(instance, skinId);
        if (ret || !g_Enabled) return ret;

        // Create fake instance if not owned
        auto newInst = (CmdHeroSkin*)Il2CppCreateClassInstance("Assembly-CSharp.dll", "MTTDProto", "CmdHeroSkin");
        if (newInst) {
            newInst->iId = skinId;
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

        void* addrSendData = Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "GameServerConfig", "SendRawData", 7);
        if (addrSendData) Tools::Hook(addrSendData, (void*)hSendRawData, (void**)&oSendRawData);
    }
}
