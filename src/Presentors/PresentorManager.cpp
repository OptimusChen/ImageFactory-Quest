#include "Presentors/PresentorManager.hpp"

namespace ImageFactory {

    StringW PresentorManager::EVERYWHERE = "Everywhere";
    StringW PresentorManager::IN_MENU = "In Menu";
    StringW PresentorManager::RESULTS_SCREEN = "Results Screen";
    StringW PresentorManager::IN_SONG = "In Song";
    StringW PresentorManager::PERCENT = "%";
    StringW PresentorManager::PERCENT_RANGE = "% Range";
    StringW PresentorManager::COMBO = "Combo";
    StringW PresentorManager::COMBO_INCREMENT = "Combo Increment";
    StringW PresentorManager::COMBO_HOLD = "Combo Hold";
    StringW PresentorManager::COMBO_DROP = "Combo Drop";
    StringW PresentorManager::FULL_COMBO = "Full Combo";
    StringW PresentorManager::IN_PAUSE = "In Pause Menu";
    StringW PresentorManager::LAST_NOTE = "On Last Note";
    std::vector<StringW> PresentorManager::PRESENTORS = {
        EVERYWHERE,    IN_MENU,  RESULTS_SCREEN,  IN_SONG,    PERCENT,
        PERCENT_RANGE, COMBO,    COMBO_INCREMENT, COMBO_HOLD, COMBO_DROP,
        FULL_COMBO,    IN_PAUSE, LAST_NOTE};

    std::unordered_map<IFImage*, StringW>* PresentorManager::MAP = new std::unordered_map<IFImage*, StringW>();

    void PresentorManager::Parse(IFImage* image, StringW str) {
        if (MAP->contains(image)) {
            MAP->erase(image);
        }
        
        MAP->insert({image, to_utf8(csstrtostr(str))});
    }

    void PresentorManager::ClearInfo(IFImage* image) {
        if (MAP->contains(image)) {
            MAP->erase(image);
        }
    }

    void PresentorManager::SpawnInMenu() {
        for (std::pair<IFImage*, StringW> pair : *MAP) {
            if (pair.second.starts_with(EVERYWHERE) ||
                pair.second.starts_with(IN_MENU)) {
                pair.first->Spawn();
            } else {
                pair.first->Despawn();
            }
        }
    }

    void PresentorManager::SpawnAll() {
        for (std::pair<IFImage*, StringW> pair : *MAP) {
            pair.first->Spawn();
        }
        }

    void PresentorManager::DespawnAll() {
        for (std::pair<IFImage*, StringW> pair : *MAP) {
            pair.first->Despawn();
        }
    }

    void PresentorManager::SpawnforAll(StringW str) {
        for (std::pair<IFImage*, StringW> pair : *MAP) {
            if (pair.second.starts_with(str)) {
                pair.first->Spawn();
            }
        }
    }

    void PresentorManager::DespawnforAll(StringW str) {
        for (std::pair<IFImage*, StringW> pair : *MAP) {
            if (pair.second.starts_with(str)) {
            pair.first->Despawn();
            }
        }
    }
}
