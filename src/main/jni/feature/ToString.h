#pragma once
#include <string>
#include <vector>
#include <map>
#include <algorithm>

namespace GameData {
    // --- Ranks ---
    const std::string Ranks[] = {
        "-", "Warrior III * 0", "Warrior III * 1", "Warrior III * 2", "Warrior III * 3",
        "Warrior II * 1", "Warrior II * 2", "Warrior II * 3",
        "Warrior I * 1", "Warrior I * 2", "Warrior I * 3",
        "Elite III * 0", "Elite III * 1", "Elite III * 2", "Elite III * 3", "Elite III * 4",
        "Elite II * 0", "Elite II * 1", "Elite II * 2", "Elite II * 3", "Elite II * 4",
        "Elite I * 0", "Elite I * 1", "Elite I * 2", "Elite I * 3", "Elite I * 4",
        "Master IV * 0", "Master IV * 1", "Master IV * 2", "Master IV * 3", "Master IV * 4",
        "Master III * 0", "Master III * 1", "Master III * 2", "Master III * 3", "Master III * 4",
        "Master II * 0", "Master II * 1", "Master II * 2", "Master II * 3", "Master II * 4",
        "Master I * 0", "Master I * 1", "Master I * 2", "Master I * 3", "Master I * 4",
        "Grandmaster V * 0", "Grandmaster V * 1", "Grandmaster V * 2", "Grandmaster V * 3", "Grandmaster V * 4", "Grandmaster V * 5",
        "Grandmaster IV * 0", "Grandmaster IV * 1", "Grandmaster IV * 2", "Grandmaster IV * 3", "Grandmaster IV * 4", "Grandmaster IV * 5",
        "Grandmaster III * 0", "Grandmaster III * 1", "Grandmaster III * 2", "Grandmaster III * 3", "Grandmaster III * 4", "Grandmaster III * 5",
        "Grandmaster II * 0", "Grandmaster II * 1", "Grandmaster II * 2", "Grandmaster II * 3", "Grandmaster II * 4", "Grandmaster II * 5",
        "Grandmaster I * 0", "Grandmaster I * 1", "Grandmaster I * 2", "Grandmaster I * 3", "Grandmaster I * 4", "Grandmaster I * 5",
        "Epic V * 0", "Epic V * 1", "Epic V * 2", "Epic V * 3", "Epic V * 4", "Epic V * 5",
        "Epic IV * 0", "Epic IV * 1", "Epic IV * 2", "Epic IV * 3", "Epic IV * 4", "Epic IV * 5",
        "Epic III * 0", "Epic III * 1", "Epic III * 2", "Epic III * 3", "Epic III * 4", "Epic III * 5",
        "Epic II * 0", "Epic II * 1", "Epic II * 2", "Epic II * 3", "Epic II * 4", "Epic II * 5",
        "Epic I * 0", "Epic I * 1", "Epic I * 2", "Epic I * 3", "Epic I * 4", "Epic I * 5",
        "Legend V * 0", "Legend V * 1", "Legend V * 2", "Legend V * 3", "Legend V * 4", "Legend V * 5",
        "Legend IV * 0", "Legend IV * 1", "Legend IV * 2", "Legend IV * 3", "Legend IV * 4", "Legend IV * 5",
        "Legend III * 0", "Legend III * 1", "Legend III * 2", "Legend III * 3", "Legend III * 4", "Legend III * 5",
        "Legend II * 0", "Legend II * 1", "Legend II * 2", "Legend II * 3", "Legend II * 4", "Legend II * 5",
        "Legend I * 0", "Legend I * 1", "Legend I * 2", "Legend I * 3", "Legend I * 4", "Legend I * 5"
    };

    inline std::string RankToString(int level, int mythicPoints = 0) {
        if (level > 135) {
            if (mythicPoints >= 100) return "Mythical Immortal (" + std::to_string(mythicPoints) + ")";
            if (mythicPoints >= 50)  return "Mythic Glory (" + std::to_string(mythicPoints) + ")";
            if (mythicPoints >= 25)  return "Mythical Honor (" + std::to_string(mythicPoints) + ")";
            return "Mythic (" + std::to_string(mythicPoints) + ")";
        }
        if (level >= 0 && level < (sizeof(Ranks)/sizeof(Ranks[0]))) return Ranks[level];
        return "Rank " + std::to_string(level);
    }

    // --- Heroes ---
    const std::string Heroes[] = {
        "-", "Miya", "Balmond", "Saber", "Alice", "Nana", "Tigreal", "Alucard", "Karina", "Akai",
        "Franco", "Bane", "Bruno", "Clint", "Rafaela", "Eudora", "Zilong", "Fanny", "Layla", "Minotaur",
        "Lolita", "Hayabusa", "Freya", "Gord", "Natalia", "Kagura", "Chou", "Sun", "Alpha", "Ruby",
        "Yi Sun-shin", "Moskov", "Johnson", "Cyclops", "Estes", "Hilda", "Aurora", "Lapu-Lapu", "Vexana", "Roger",
        "Karrie", "Gatotkaca", "Harley", "Irithel", "Grock", "Argus", "Odette", "Lancelot", "Diggie", "Hylos",
        "Zhask", "Helcurt", "Pharsa", "Lesley", "Jawhead", "Angela", "Gusion", "Valir", "Martis", "Uranus",
        "Hanabi", "Chang'e", "Kaja", "Selena", "Aldous", "Claude", "Vale", "Leomord", "Lunox", "Hanzo",
        "Belerick", "Kimmy", "Thamuz", "Harith", "Minsitthar", "Kadita", "Faramis", "Badang", "Khufra", "Granger",
        "Guinevere", "Esmeralda", "Terizla", "X.Borg", "Ling", "Dyrroth", "Lylia", "Baxia", "Masha", "Wanwan",
        "Silvanna", "Cecilion", "Carmilla", "Atlas", "Popol & Kupa", "Yu Zhong", "Luo Yi", "Benedetta", "Khaleed", "Barats",
        "Brody", "Yve", "Mathilda", "Paquito", "Gloo", "Beatrix", "Phoveus", "Natan", "Aulus", "Aamon",
        "Valentina", "Edith", "Floryn", "Yin", "Melissa", "Xavier", "Julian", "Fredrinn", "Joy", "Arlott", "Novaria", "Ixia"
    };

    inline std::string HeroToString(int id) {
        if (id >= 0 && id < (sizeof(Heroes)/sizeof(Heroes[0]))) return Heroes[id];
        return "Hero " + std::to_string(id);
    }

    // --- Spells ---
    inline std::string SpellToString(int id) {
        static std::map<int, std::string> spells = {
            {20150, "Execute"}, {20020, "Retribution"}, {20030, "Inspire"}, {20040, "Sprint"},
            {20050, "Revitalize"}, {20060, "Aegis"}, {20070, "Petrify"}, {20080, "Purify"},
            {20140, "Flameshot"}, {20100, "Flicker"}, {20160, "Arrival"}, {20190, "Vengeance"}
        };
        if (spells.count(id)) return spells[id];
        return (id == 0) ? "-" : std::to_string(id);
    }
}
