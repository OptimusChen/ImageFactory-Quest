#pragma once

#include "IFImage.hpp"
#include "beatsaber-hook/shared/utils/typedefs-string.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "Presenter.hpp"

#include <map>
#include <vector>

namespace ImageFactory {
    class PresenterManager {
        public:
            static void Parse(IFImage* image, StringW pres);
            static void SpawnInMenu();
            static void SpawnAll();
            static void DespawnAll();
            static void SpawnforAll(StringW pres);
            static void DespawnforAll(StringW pres);
            static custom_types::Helpers::Coroutine DespawnAfter(IFImage* image, float time);
            static void ClearInfo(IFImage* image);
            static Presenters::Presenter* GetPresenter(std::string pres);
            static void SpawnExtraData(std::string pres, std::string key, std::string val);
            static void DespawnExtraData(std::string pres, std::string key, std::string val);

            static std::string EVERYWHERE;
            static std::string IN_MENU;
            static std::string RESULTS_SCREEN;
            static std::string IN_SONG;
            static std::string PERCENT;
            static std::string PERCENT_RANGE;
            static std::string COMBO;
            static std::string COMBO_INCREMENT;
            static std::string COMBO_HOLD;
            static std::string COMBO_DROP;
            static std::string FULL_COMBO;
            static std::string IN_PAUSE;
            static std::string LAST_NOTE;
            static std::vector<std::string> PRESENTERS;
            static std::unordered_map<IFImage*, std::string>* MAP;
    };
}