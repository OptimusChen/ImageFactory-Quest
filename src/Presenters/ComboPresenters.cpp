#include "main.hpp"

#include "Presenters/PresenterManager.hpp"
#include "Utils/StringUtils.hpp"
#include "Presenters/ComboPresenters.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "System/Action.hpp"
#include "System/Action_1.hpp"
#include "GlobalNamespace/ComboController.hpp"

using namespace QuestUI;

namespace ImageFactory::Presenters {

    std::vector<GameObject*> ComboPresenter::GetUIElements(Transform* parent, IFImage* image) {
        std::vector<GameObject*> ret;

        auto combo = BeatSaberUI::CreateIncrementSetting(parent, "Combo", 0, 1, std::stof(image->GetExtraData("combo_combo", "100")), 0, 10000, 
            [=](float f){
                image->SetExtraData("combo_combo", StringUtils::removeTrailingZeros(round(f)));
            })->get_gameObject();

        auto duration = BeatSaberUI::CreateIncrementSetting(parent, "Duration (Seconds)", 2, 0.25, std::stof(image->GetExtraData("combo_duration", "1")), 0.25, 100, 
            [=](float f){
                image->SetExtraData("combo_duration", StringUtils::removeTrailingZeros(round(f)));
            })->get_gameObject();

        ret.push_back(combo);
        ret.push_back(duration);

        return ret;
    }

    std::vector<GameObject*> ComboIncrementPresenter::GetUIElements(Transform* parent, IFImage* image) {
        std::vector<GameObject*> ret;

        auto combo = BeatSaberUI::CreateIncrementSetting(parent, "On Every X Combo", 0, 1, std::stof(image->GetExtraData("combo_inc_combo", "100")), 0, 10000,
            [=](float f){
                image->SetExtraData("combo_inc_combo", StringUtils::removeTrailingZeros(round(f)));
            })->get_gameObject();

        auto duration = BeatSaberUI::CreateIncrementSetting(parent, "Duration (Seconds)", 2, 0.25, std::stof(image->GetExtraData("combo_inc_duration", "1")), 0, 100, 
            [=](float f){
                image->SetExtraData("combo_inc_duration", StringUtils::removeTrailingZeros(round(f)));
            })->get_gameObject();

        ret.push_back(combo);
        ret.push_back(duration);

        return ret;
    }

    std::vector<GameObject*> ComboHoldPresenter::GetUIElements(Transform* parent, IFImage* image) {
        std::vector<GameObject*> ret;

        auto dropDown = BeatSaberUI::CreateDropdown(parent, "When", image->GetExtraData("combo_hold_when", "Below"), {"Below", "Above"}, 
            [=](StringW s){
                image->SetExtraData("combo_hold_when", s);  
            })->get_transform()->get_parent()->get_gameObject();

        auto combo = BeatSaberUI::CreateIncrementSetting(parent, "Combo", 0, 1, std::stof(image->GetExtraData("combo_hold_combo", "100")), 0, 10000, 
            [=](float f){
                image->SetExtraData("combo_hold_combo", StringUtils::removeTrailingZeros(round(f)));
            })->get_gameObject();

        ret.push_back(combo);
        ret.push_back(dropDown);

        return ret;
    }

    std::vector<GameObject*> ComboDropPresenter::GetUIElements(Transform* parent, IFImage* image) {
        std::vector<GameObject*> ret;

        auto duration = BeatSaberUI::CreateIncrementSetting(parent, "Duration (Seconds)", 2, 0.25, std::stof(image->GetExtraData("combo_drop_duration", "1")), 0, 100, 
            [=](float f){
                image->SetExtraData("combo_drop_duration", StringUtils::removeTrailingZeros(round(f)));
            })->get_gameObject();

        ret.push_back(duration);

        return ret;
    }

    MAKE_HOOK_MATCH(ComboController_Start, &ComboController::Start, void, ComboController* self) {
        ComboController_Start(self);

        self->add_comboBreakingEventHappenedEvent(custom_types::MakeDelegate<System::Action*>(std::function([=](){
            if (!UIUtils::NoHud()) return; 

            PresenterManager::DespawnforAll(PresenterManager::FULL_COMBO);

            for (std::pair<IFImage*, StringW> pair : *PresenterManager::MAP) {
                if (pair.second.starts_with(PresenterManager::COMBO_DROP)) {
                    StartCoroutine(PresenterManager::DespawnAfter(pair.first, stof(pair.first->GetExtraData("combo_drop_duration", "1"))));
                }
            }
            
            Presenter::fullCombo = false;
        })));

        self->add_comboDidChangeEvent(custom_types::MakeDelegate<System::Action_1<int>*>(std::function([=](int combo){
            if (!UIUtils::NoHud()) return;
            
            for (std::pair<IFImage*, StringW> pair : *PresenterManager::MAP) {
                IFImage* image = pair.first;

                if (pair.second.starts_with(PresenterManager::COMBO) &&
                    combo == stoi(pair.first->GetExtraData("combo_combo", "100"))) {
                    StartCoroutine(PresenterManager::DespawnAfter(image, stof(pair.first->GetExtraData("combo_duration", "1"))));
                }

                if (pair.second.starts_with(PresenterManager::COMBO_INCREMENT) &&
                    combo % stoi(image->GetExtraData("combo_inc_combo", "100")) == 0 &&
                    combo > 0) {

                    StartCoroutine(PresenterManager::DespawnAfter(image, stof(pair.first->GetExtraData("combo_inc_duration", "1"))));
                }

                if (pair.second.starts_with(PresenterManager::COMBO_HOLD)) {
                    if (image->GetExtraData("combo_hold_when", "Below").starts_with("Above")) {
                        if (combo > stoi(image->GetExtraData("combo_hold_combo", "100"))) {
                            image->Spawn(true);
                        } else {
                            image->Despawn(true);
                        }
                    } else {
                        if (combo < stoi(image->GetExtraData("combo_hold_combo", "100"))) {
                            image->Spawn(true);
                        } else {
                            image->Despawn(true);
                        }
                    }
                }
            }
        })));
    }

    void ComboHooks() {
        INSTALL_HOOK(getLogger(), ComboController_Start);
    }
}