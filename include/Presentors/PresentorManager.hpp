#pragma once

#include "IFImage.hpp"
#include "beatsaber-hook/shared/utils/typedefs-string.hpp"

#include <map>
#include <vector>

namespace ImageFactory {
    class PresentorManager {
        public:
            static void Parse(IFImage* image, StringW pres);
            static void SpawnInMenu();
            static void SpawnAll();
            static void DespawnAll();
            static void SpawnforAll(StringW pres);
            static void DespawnforAll(StringW pres);
            static void ClearInfo(IFImage* image);
            // static void SpawnforAllExtraVal(StringW pres, StringW key, StringW val);
            // static void DespawnforAllExtraVal(StringW pres, StringW key, StringW val);

            static StringW EVERYWHERE;
            static StringW IN_MENU;
            static StringW RESULTS_SCREEN;
            static StringW IN_SONG;
            static StringW PERCENT;
            static StringW PERCENT_RANGE;
            static StringW COMBO;
            static StringW COMBO_INCREMENT;
            static StringW COMBO_HOLD;
            static StringW COMBO_DROP;
            static StringW FULL_COMBO;
            static StringW IN_PAUSE;
            static StringW LAST_NOTE;
            static std::vector<StringW> PRESENTORS;
            static std::unordered_map<IFImage*, StringW>* MAP;
    };
}