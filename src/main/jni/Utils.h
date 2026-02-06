#ifndef UTILS_H
#define UTILS_H

#include <jni.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <android/log.h>

/**
 * Optimized utility functions for memory and library management.
 */

namespace Utils {
    inline uintptr_t GetLibraryBase(const char *library) {
        static uintptr_t address = 0;
        if (address != 0) return address;

        FILE *fp = fopen("/proc/self/maps", "rt");
        if (fp == nullptr) return 0;

        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, library)) {
                address = (uintptr_t)strtoul(buffer, nullptr, 16);
                break;
            }
        }
        fclose(fp);
        return address;
    }

    inline bool IsLibraryLoaded(const char *libraryName) {
        return GetLibraryBase(libraryName) != 0;
    }

    inline uintptr_t GetAbsoluteAddress(const char *libraryName, uintptr_t relativeAddr) {
        uintptr_t base = GetLibraryBase(libraryName);
        return (base == 0) ? 0 : (base + relativeAddr);
    }
}

// Global legacy compatibility (if needed)
typedef uintptr_t DWORD;

#endif