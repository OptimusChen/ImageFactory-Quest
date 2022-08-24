#include "main.hpp"

#include "Presenters/PresenterManager.hpp"
#include "Presenters/ResultsScreenPresenter.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/LevelCompletionResults.hpp"

using namespace QuestUI;
using namespace GlobalNamespace;

namespace ImageFactory::Presenters {

    std::vector<GameObject*> ResultsScreenPresenter::GetUIElements(Transform* parent, IFImage* image) {
        std::vector<GameObject*> ret;

        auto dropDown = BeatSaberUI::CreateDropdown(parent, "When", image->GetExtraData("results_when", "Finished"), {"Finished", "Passed", "Failed"}, 
            [=](StringW s){
                image->SetExtraData("results_when", s);  
            });

        ret.push_back(dropDown->get_transform()->get_parent()->get_gameObject());

        return ret;
    }

    MAKE_HOOK_MATCH(ResultsViewController_DidActivate, &ResultsViewController::DidActivate, void, ResultsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        ResultsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

        if (self->levelCompletionResults->energy < 1) {
            PresenterManager::SpawnExtraData(PresenterManager::RESULTS_SCREEN, "results_when", "Failed");
        } else {
            PresenterManager::SpawnExtraData(PresenterManager::RESULTS_SCREEN, "results_when", "Passed");
        }
        
        PresenterManager::SpawnExtraData(PresenterManager::RESULTS_SCREEN, "results_when", "Finished");
    }

    MAKE_HOOK_MATCH(ResultsViewController_DidDeactivate, &ResultsViewController::DidDeactivate, void, ResultsViewController* self, bool addedToHierarchy, bool screenSystemEnabling) {
        ResultsViewController_DidDeactivate(self, addedToHierarchy, screenSystemEnabling);
        
        PresenterManager::DespawnforAll(PresenterManager::RESULTS_SCREEN);
    }

    void ResultsHooks() {
        INSTALL_HOOK(getLogger(), ResultsViewController_DidActivate);
        INSTALL_HOOK(getLogger(), ResultsViewController_DidDeactivate);
    }
}