#pragma once
#include "../Include.h"
#include "../include/Utils/Unity/ByNameModding/Tools.h"
#include "../include/Utils/Unity/ByNameModding/Il2Cpp.h"
#include "../include/Utils/Logger.h"

namespace UnlockSkin {

    // ================= GLOBAL CONFIG =================
    bool g_UnlockAll = true;

    uint32_t m_HeroID = 0;
    uint32_t m_SkinID = 0;

    // ================= OFFSETS =================
    static size_t off_Skin_iId = 0;
    static size_t off_Skin_iLimitTime = 0;
    static size_t off_Skin_iSource = 0;
    static size_t off_Skin_iLimitTimeMagicChess = 0;
    static size_t off_Skin_iGetTime = 0;

    static size_t off_PlayerInfo_uiSkinId = 0;
    static size_t off_PlayerInfo_uiSelHero = 0;
    static size_t off_PlayerInfo_iCamp = 0;

    // ================= STRUCTS =================
    // We use void* and offsets for flexibility
    struct CmdHeroSkin {
        // Placeholder
    };

    // ================= ORIGINAL FUNCTIONS =================
    void* (*oGetHeroSkin)(void* unk, void* list, uint32_t skinid);
    void* (*oIsHaveSkin)(void* unk, int32_t skinid);
    void* (*oIsHaveSkinForever)(void* unk, int32_t skinid);

    bool (*oIsForbidSkin)(void* unk, uint32_t skinid, bool filterLuaCheck);
    bool (*oIsForbidHeros)(void* unk, uint32_t heroid);
    bool (*oIsLimitActiveHero)(void* unk, uint32_t heroid);
    bool (*oIsHeroInShop)(void* unk, uint32_t heroid);
    bool (*oIsCanUseSkin)(void* unk, int32_t heroid);

    uint32_t (*oGetExpiredCardSkinLimitTime)(void* unk, uint32_t skinId);
    void (*oRefreshSkinDic)(void* thiz, uint32_t heroId, uint32_t skinId, uint64_t lUid);

    void (*oSendSelectSkin)(void* thiz, uint32_t skinid, uint32_t heroid);
    void (*oSendUseSkin)(void* thiz, int32_t skinid, bool bForce);
    void (*oSetPlayerData)(void* thiz, void* playerinfo, uint32_t selfCamp);

    // ================= HELPER FUNCTIONS =================
    void InitOffsets() {
        if (off_Skin_iId == 0) {
            off_Skin_iId = Il2CppGetFieldOffset("Assembly-CSharp.dll", "MTTDProto", "CmdHeroSkin", "iId");
            off_Skin_iLimitTime = Il2CppGetFieldOffset("Assembly-CSharp.dll", "MTTDProto", "CmdHeroSkin", "iLimitTime");
            off_Skin_iSource = Il2CppGetFieldOffset("Assembly-CSharp.dll", "MTTDProto", "CmdHeroSkin", "iSource");
            off_Skin_iLimitTimeMagicChess = Il2CppGetFieldOffset("Assembly-CSharp.dll", "MTTDProto", "CmdHeroSkin", "iLimitTimeMagicChess");
            off_Skin_iGetTime = Il2CppGetFieldOffset("Assembly-CSharp.dll", "MTTDProto", "CmdHeroSkin", "iGetTime");

            off_PlayerInfo_uiSkinId = Il2CppGetFieldOffset("Assembly-CSharp.dll", "MTTDProto", "BattlePlayerInfo", "uiSkinId");
            off_PlayerInfo_uiSelHero = Il2CppGetFieldOffset("Assembly-CSharp.dll", "MTTDProto", "BattlePlayerInfo", "uiSelHero");
            off_PlayerInfo_iCamp = Il2CppGetFieldOffset("Assembly-CSharp.dll", "MTTDProto", "BattlePlayerInfo", "iCamp");
        }
    }

    void* CreateFakeSkin(uint32_t skinId) {
        InitOffsets();
        void* instance = Il2CppCreateClassInstance("Assembly-CSharp.dll", "MTTDProto", "CmdHeroSkin");
        if (instance) {
            if (off_Skin_iId) *(uint32_t*)((uintptr_t)instance + off_Skin_iId) = skinId;
            if (off_Skin_iLimitTime) *(uint32_t*)((uintptr_t)instance + off_Skin_iLimitTime) = 0; // 0 = Permanent
            if (off_Skin_iSource) *(uint32_t*)((uintptr_t)instance + off_Skin_iSource) = 1;
            if (off_Skin_iLimitTimeMagicChess) *(uint32_t*)((uintptr_t)instance + off_Skin_iLimitTimeMagicChess) = 0;
            if (off_Skin_iGetTime) *(uint32_t*)((uintptr_t)instance + off_Skin_iGetTime) = 0;
        }
        return instance;
    }

    // ================= HOOKS =================

    // --- Data Provider ---
    void* hGetHeroSkin(void* unk, void* list, uint32_t skinid) {
        void* ret = oGetHeroSkin(unk, list, skinid);
        if (g_UnlockAll) {
            if (ret == nullptr) return CreateFakeSkin(skinid);
            return CreateFakeSkin(skinid);
        }
        return ret;
    }

    void* hIsHaveSkin(void* unk, int32_t skinid) {
        void* ret = oIsHaveSkin(unk, skinid);
        if (g_UnlockAll) {
            return CreateFakeSkin((uint32_t)skinid);
        }
        return ret;
    }

    void* hIsHaveSkinForever(void* unk, int32_t skinid) {
        void* ret = oIsHaveSkinForever(unk, skinid);
        if (g_UnlockAll) {
            return CreateFakeSkin((uint32_t)skinid);
        }
        return ret;
    }

    // --- Bypass & Limit ---
    bool hIsForbidSkin(void* unk, uint32_t skinid, bool filterLuaCheck) {
        if (g_UnlockAll) return false;
        return oIsForbidSkin(unk, skinid, filterLuaCheck);
    }

    bool hIsForbidHeros(void* unk, uint32_t heroid) {
        if (g_UnlockAll) return false;
        return oIsForbidHeros(unk, heroid);
    }

    bool hIsLimitActiveHero(void* unk, uint32_t heroid) {
        if (g_UnlockAll) return false;
        return oIsLimitActiveHero(unk, heroid);
    }

    bool hIsHeroInShop(void* unk, uint32_t heroid) {
        if (g_UnlockAll) return true;
        return oIsHeroInShop(unk, heroid);
    }

    bool hIsCanUseSkin(void* unk, int32_t heroid) {
        if (g_UnlockAll) return true;
        return oIsCanUseSkin(unk, heroid);
    }

    uint32_t hGetExpiredCardSkinLimitTime(void* unk, uint32_t skinId) {
        if (g_UnlockAll) return 0;
        return oGetExpiredCardSkinLimitTime(unk, skinId);
    }

    void hRefreshSkinDic(void* thiz, uint32_t heroId, uint32_t skinId, uint64_t lUid) {
        oRefreshSkinDic(thiz, heroId, skinId, lUid);
    }

    // --- Selection Spoofing ---
    void hSendSelectSkin(void* thiz, uint32_t skinid, uint32_t heroid) {
        if (g_UnlockAll) {
            m_HeroID = heroid;
            m_SkinID = skinid;
            oSendSelectSkin(thiz, 0, heroid);
            return;
        }
        oSendSelectSkin(thiz, skinid, heroid);
    }

    void hSendUseSkin(void* thiz, int32_t skinId, bool bForce) {
        if (g_UnlockAll) {
            m_SkinID = (uint32_t)skinId;
            oSendUseSkin(thiz, 0, bForce);
            return;
        }
        oSendUseSkin(thiz, skinId, bForce);
    }

    // --- Injection ---
    void hSetPlayerData(void* thiz, void* playerinfo, uint32_t selfCamp) {
        // Call original first to populate data
        oSetPlayerData(thiz, playerinfo, selfCamp);

        if (g_UnlockAll && m_SkinID != 0 && playerinfo != nullptr) {
            InitOffsets();

            uint32_t playerSelHero = 0;
            if (off_PlayerInfo_uiSelHero) {
                playerSelHero = *(uint32_t*)((uintptr_t)playerinfo + off_PlayerInfo_uiSelHero);
            }

            uint32_t playerCamp = 0;
            if (off_PlayerInfo_iCamp) {
                playerCamp = *(uint32_t*)((uintptr_t)playerinfo + off_PlayerInfo_iCamp);
            }

            // If hero matches and camp matches selfCamp (which is passed in)
            if (playerSelHero == m_HeroID && playerCamp == selfCamp) {
                if (off_PlayerInfo_uiSkinId) {
                    *(uint32_t*)((uintptr_t)playerinfo + off_PlayerInfo_uiSkinId) = m_SkinID;
                }
            }
        }
    }

    // ================= INITIALIZATION =================
    void Init() {
        // Data Provider
        // Il2CppGetMethodOffset(image, namespace, class, method, args)
        Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", "GetHeroSkin", 2), (void*)hGetHeroSkin, (void**)&oGetHeroSkin);
        Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", "IsHaveSkin", 1), (void*)hIsHaveSkin, (void**)&oIsHaveSkin);
        Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", "IsHaveSkinForever", 1), (void*)hIsHaveSkinForever, (void**)&oIsHaveSkinForever);

        // Bypass
        Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", "IsForbidSkin", 2), (void*)hIsForbidSkin, (void**)&oIsForbidSkin);
        Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", "IsForbidHeros", 1), (void*)hIsForbidHeros, (void**)&oIsForbidHeros);
        Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", "IsLimitActiveHero", 1), (void*)hIsLimitActiveHero, (void**)&oIsLimitActiveHero);
        Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", "IsHeroInShop", 1), (void*)hIsHeroInShop, (void**)&oIsHeroInShop);
        Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", "IsCanUseSkin", 1), (void*)hIsCanUseSkin, (void**)&oIsCanUseSkin);

        Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", "GetExpiredCardSkinLimitTime", 1), (void*)hGetExpiredCardSkinLimitTime, (void**)&oGetExpiredCardSkinLimitTime);
        Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "UIRankHero", "RefreshSkinDic", 3), (void*)hRefreshSkinDic, (void**)&oRefreshSkinDic);

        // Selection Spoofing
        Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "ChooseHeroMgr", "SendSelectSkin", 2), (void*)hSendSelectSkin, (void**)&oSendSelectSkin);
        // SendUseSkin is in private class UIRankHero.ChangeShow (UIRankHero+ChangeShow)
        Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "UIRankHero+ChangeShow", "SendUseSkin", 2), (void*)hSendUseSkin, (void**)&oSendUseSkin);

        // Injection
        Tools::Hook(Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "BattleReceiveMessage", "SetPlayerData", 2), (void*)hSetPlayerData, (void**)&oSetPlayerData);
    }
}
