#include "main.hpp"

#include "Presenters/PresenterManager.hpp"
#include "Utils/StringUtils.hpp"
#include "Presenters/PercentPresenters.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "GlobalNamespace/ScoreController.hpp"
#include "GlobalNamespace/RelativeScoreAndImmediateRankCounter.hpp"
#include "System/Action.hpp"

using namespace QuestUI;

namespace ImageFactory::Presenters {

    std::vector<GameObject*> PercentPresenter::GetUIElements(Transform* parent, IFImage* image) {
        std::vector<GameObject*> ret;

        auto dropDown = BeatSaberUI::CreateDropdown(parent, "When", image->GetExtraData("percent_when", "Below"), {"Below", "Above"}, 
            [=](StringW s){
                image->SetExtraData("percent_when", s);  
            })->get_transform()->get_parent()->get_gameObject();

        auto percent = BeatSaberUI::CreateIncrementSetting(parent, "%", 0, 1, std::stof(image->GetExtraData("percent_%", "80")), 0, 100, 
            [=](float f){
                image->SetExtraData("percent_%", StringUtils::removeTrailingZeros(round(f)));
            })->get_gameObject();

        ret.push_back(dropDown);
        ret.push_back(percent);

        return ret;
    }

    std::vector<GameObject*> PercentRangePresenter::GetUIElements(Transform* parent, IFImage* image) {
        std::vector<GameObject*> ret;

        auto above = BeatSaberUI::CreateIncrementSetting(parent, "When above (%)", 0, 1, std::stof(image->GetExtraData("percentrange_above", "80")), 0, 100, 
            [=](float f){
                image->SetExtraData("percentrange_above", StringUtils::removeTrailingZeros(round(f)));
            })->get_gameObject();

        auto below = BeatSaberUI::CreateIncrementSetting(parent, "When below (%)", 0, 1, std::stof(image->GetExtraData("percentrange_below", "90")), 0, 100, 
            [=](float f){
                image->SetExtraData("percentrange_below", StringUtils::removeTrailingZeros(round(f)));
            })->get_gameObject();

        ret.push_back(above);
        ret.push_back(below);

        return ret;
    }

    MAKE_HOOK_MATCH(ScoreController_Start, &ScoreController::Start, void, ScoreController* self) {
        ScoreController_Start(self);

        auto rankCounter = Object::FindObjectOfType<RelativeScoreAndImmediateRankCounter*>();

        rankCounter->add_relativeScoreOrImmediateRankDidChangeEvent(custom_types::MakeDelegate<System::Action*>(std::function([=](){
            if (!UIUtils::NoHud()) return;
            
            int score = rankCounter->relativeScore * 100;
            
            double percentage = (double) score;

            for (std::pair<IFImage*, StringW> pair : *PresenterManager::MAP) {
                IFImage* image = pair.first;
                
                if (pair.second.starts_with(PresenterManager::PERCENT)) {
                    if (image->GetExtraData("percent_when", "Below").starts_with("Above")) {
                        if (percentage > stof(image->GetExtraData("percent_%", "80"))) {
                            image->Spawn(true);
                        } else {
                            image->Despawn(true);
                        }
                    } else if (image->GetExtraData("percent_when", "Below").starts_with("Below")) {
                        if (percentage < stof(image->GetExtraData("percent_%", "80"))) {
                            image->Spawn(true);
                        } else {
                            image->Despawn(true);
                        }
                    }
                }

                if (pair.second.starts_with(PresenterManager::PERCENT_RANGE)) {
                    if (percentage > stof(image->GetExtraData("percentrange_above", "80")) &&
                        percentage < stof(image->GetExtraData("percentrange_below", "90"))) {
                      image->Spawn(true);
                    } else {
                        image->Despawn(true);
                    }
                }
            }
        })));
    }

    void PercentHooks() {
        INSTALL_HOOK(getLogger(), ScoreController_Start);
    }
}