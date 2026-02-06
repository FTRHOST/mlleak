#pragma once
#include "../include/Utils/Unity/ByNameModding/Tools.h"
#include "../include/Utils/Unity/ByNameModding/Il2Cpp.h"

/**
 * TEMPLATE MOD SCRIPT
 * Gunakan file ini sebagai referensi saat membuat fitur baru.
 */
namespace FiturBaru {
    extern bool g_Enabled;

    // =========================================================================
    // 1. CONTOH HOOK BOOLEAN (Paling Sering Digunakan)
    // Digunakan untuk fungsi "Apakah..." (Is...) atau "Punya..." (Has...)
    // =========================================================================
    bool (*oIsTargetVisible)(void* instance);
    bool hIsTargetVisible(void* instance) {
        if (g_Enabled) {
            return true; // Paksa game menganggap musuh selalu terlihat (Map Hack)
        }
        return oIsTargetVisible(instance);
    }

    // =========================================================================
    // 2. CONTOH HOOK VOID (Untuk Fungsi Aksi)
    // Digunakan untuk menjalankan kode tambahan saat sesuatu terjadi
    // =========================================================================
    void (*oOnPlayerDeath)(void* instance, void* killer);
    void hOnPlayerDeath(void* instance, void* killer) {
        // Jalankan fungsi asli game dulu agar tidak crash
        oOnPlayerDeath(instance, killer);

        // Tambahkan logika kita sendiri (misal: Log ke logcat)
        // LOGI("Ada player yang mati!");
    }

    // =========================================================================
    // 3. CONTOH HOOK INTEGER (Untuk Nilai Angka)
    // =========================================================================
    int (*oGetPlayerGold)(void* instance);
    int hGetPlayerGold(void* instance) {
        int originalGold = oGetPlayerGold(instance);
        if (g_Enabled) {
            return originalGold + 1000; // Tambahkan bonus emas visual
        }
        return originalGold;
    }

    // 4. Inisialisasi Hook
    inline void Init() {
        if (!g_Enabled) return; // Jangan cari offset jika fitur dimatikan

        // Contoh penerapan hook untuk Boolean
        void* addrVisible = Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "UnitInstance", "get_IsVisible", 0);
        if (addrVisible) Tools::Hook(addrVisible, (void*)hIsTargetVisible, (void**)&oIsTargetVisible);

        // Contoh penerapan hook untuk Void
        void* addrDeath = Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "UnitInstance", "OnDeath", 1);
        if (addrDeath) Tools::Hook(addrDeath, (void*)hOnPlayerDeath, (void**)&oOnPlayerDeath);
    }
}
