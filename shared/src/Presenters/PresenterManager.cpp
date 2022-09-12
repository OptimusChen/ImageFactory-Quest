#include "Presenters/PresenterManager.hpp"

#include "Presenters/ComboPresenters.hpp"
#include "Presenters/LastNotePresenter.hpp"
#include "Presenters/PercentPresenters.hpp"
#include "Presenters/ResultsScreenPresenter.hpp"
#include "UnityEngine/WaitForSeconds.hpp"

using namespace UnityEngine;

namespace ImageFactory {

    std::string PresenterManager::EVERYWHERE = "Everywhere";
    std::string PresenterManager::IN_MENU = "In Menu";
    std::string PresenterManager::RESULTS_SCREEN = "Results Screen";
    std::string PresenterManager::IN_SONG = "In Song";
    std::string PresenterManager::PERCENT = "%";
    std::string PresenterManager::PERCENT_RANGE = "% Range";
    std::string PresenterManager::COMBO = "Combo";
    std::string PresenterManager::COMBO_INCREMENT = "Combo Increment";
    std::string PresenterManager::COMBO_HOLD = "Combo Hold";
    std::string PresenterManager::COMBO_DROP = "Combo Drop";
    std::string PresenterManager::FULL_COMBO = "Full Combo";
    std::string PresenterManager::IN_PAUSE = "In Pause Menu";
    std::string PresenterManager::LAST_NOTE = "On Last Note";
    std::vector<std::string> PresenterManager::PRESENTERS = {
        EVERYWHERE,    IN_MENU,  RESULTS_SCREEN,  IN_SONG,    PERCENT,
        PERCENT_RANGE, COMBO,    COMBO_INCREMENT, COMBO_HOLD, COMBO_DROP,
        FULL_COMBO,    IN_PAUSE, LAST_NOTE};

    std::unordered_map<IFImage*, std::string>* PresenterManager::MAP = new std::unordered_map<IFImage*, std::string>();

    void PresenterManager::Parse(IFImage* image, StringW str) {
        if (MAP->contains(image)) {
            MAP->erase(image);
        }
        
        MAP->insert({image, str});
    }

    void PresenterManager::ClearInfo(IFImage* image) {
        if (MAP->contains(image)) {
            MAP->erase(image);
        }
    }

    void PresenterManager::SpawnInMenu() {
        for (std::pair<IFImage*, StringW> pair : *MAP) {
            if (pair.second.starts_with(EVERYWHERE) ||
                pair.second.starts_with(IN_MENU)) {
                if (!pair.first) return;
                pair.first->Spawn();
            } else {
                if (!pair.first) return;
                pair.first->Despawn();
            }
        }
    }

    void PresenterManager::SpawnAll() {
        for (std::pair<IFImage*, StringW> pair : *MAP) {
            if (!pair.first) return;
            pair.first->Spawn();
        }
        }

    void PresenterManager::DespawnAll() {
        for (std::pair<IFImage*, StringW> pair : *MAP) {
            if (!pair.first) return;
            pair.first->Despawn();
        }
    }

    void PresenterManager::SpawnforAll(StringW str) {
        for (std::pair<IFImage*, StringW> pair : *MAP) {
            if (pair.second.starts_with(str)) {
                if (!pair.first) return;
                pair.first->Spawn();
            }
        }
    }

    void PresenterManager::DespawnforAll(StringW str) {
        for (std::pair<IFImage*, StringW> pair : *MAP) {
            if (pair.second.starts_with(str)) {
                if (!pair.first) return;
                pair.first->Despawn();
            }
        }
    }

    void PresenterManager::SpawnExtraData(std::string str, std::string key, std::string val) {
        for (std::pair<IFImage*, std::string> pair : *MAP) {
            if (pair.second.compare(str) == 0) {
                if (pair.first->GetExtraData(key, val).compare(val) == 0) {
                    pair.first->Spawn();
                }
            }
        }
    }

    void PresenterManager::DespawnExtraData(std::string str, std::string key, std::string val) {
        for (std::pair<IFImage*, std::string> pair : *MAP) {
            if (pair.second.compare(str) == 0) {
                if (pair.first->GetExtraData(key, val).compare(val) == 0) {
                    pair.first->Despawn();
                }
            }
        }
    }

    custom_types::Helpers::Coroutine PresenterManager::DespawnAfter(IFImage* image, float time) {
        image->Spawn();
        co_yield reinterpret_cast<System::Collections::IEnumerator*>( CRASH_UNLESS(WaitForSeconds::New_ctor(time)));
        image->Despawn();
    }

    Presenters::Presenter* PresenterManager::GetPresenter(std::string pres) {
        if (pres.starts_with("Combo Drop")) {
            return new Presenters::ComboDropPresenter();
        } else if (pres.starts_with("Combo Hold")) {
            return new Presenters::ComboHoldPresenter();
        } else if (pres.starts_with("Combo Increment")) {
            return new Presenters::ComboIncrementPresenter();
        } else if (pres.starts_with("Combo")) {
            return new Presenters::ComboPresenter();
        } else if (pres.starts_with("% Range")) {
            return new Presenters::PercentRangePresenter();
        } else if (pres.starts_with("On Last Note")) {
            return new Presenters::LastNotePresenter();
        } else if (pres.starts_with("%")) {
            return new Presenters::PercentPresenter();
        } else if (pres.starts_with("Results Screen")) {
            return new Presenters::ResultsScreenPresenter();
        }

        return new Presenters::Presenter();
    }
}
