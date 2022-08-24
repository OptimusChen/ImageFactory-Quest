#include "Presenters/PresenterManager.hpp"

#include "main.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp"
#include "GlobalNamespace/PauseController.hpp"
#include "GlobalNamespace/PauseMenuManager.hpp"

using namespace GlobalNamespace;

namespace ImageFactory::Presenters {

    bool Presenter::fullCombo = true;
    int Presenter::currentNoteCount = 0;

    MAKE_HOOK_MATCH(AudioTimeSyncController_StartSong, &AudioTimeSyncController::StartSong, void, AudioTimeSyncController* self, float startTimeOffset) {
        AudioTimeSyncController_StartSong(self, startTimeOffset);

        PresenterManager::SpawnforAll(PresenterManager::IN_SONG);
        PresenterManager::SpawnforAll(PresenterManager::FULL_COMBO);
        PresenterManager::DespawnforAll(PresenterManager::IN_MENU);

        Presenter::currentNoteCount = 0;
        Presenter::fullCombo = true;
    }

    MAKE_HOOK_MATCH(PauseMenuManager_ShowMenu, &PauseMenuManager::ShowMenu, void, PauseMenuManager* self) {
        PresenterManager::SpawnforAll(PresenterManager::IN_PAUSE);
        PauseMenuManager_ShowMenu(self);
    }

    MAKE_HOOK_MATCH(PauseMenuManager_MenuButtonPressed, &PauseMenuManager::MenuButtonPressed, void, PauseMenuManager* self) {
        PresenterManager::DespawnforAll(PresenterManager::IN_PAUSE);
        PresenterManager::DespawnforAll(PresenterManager::IN_SONG);
        PauseMenuManager_MenuButtonPressed(self);

        Presenter::currentNoteCount = 0;
    }

    MAKE_HOOK_MATCH(PauseMenuManager_RestartButtonPressed, &PauseMenuManager::RestartButtonPressed, void, PauseMenuManager* self) {
        PresenterManager::DespawnforAll(PresenterManager::IN_PAUSE);
        PresenterManager::SpawnforAll(PresenterManager::FULL_COMBO);
        PauseMenuManager_RestartButtonPressed(self);

        Presenter::currentNoteCount = 0;
        Presenter::fullCombo = true;
    }

    MAKE_HOOK_MATCH(PauseAnimationFinish, &PauseController::HandlePauseMenuManagerDidFinishResumeAnimation, void, PauseController* self) {
        PresenterManager::DespawnforAll(PresenterManager::IN_PAUSE);
        PauseAnimationFinish(self);
    }
    
    MAKE_HOOK_MATCH(StandardLevelScenesTransitionSetupDataSO_Finish, &StandardLevelScenesTransitionSetupDataSO::Finish, void, StandardLevelScenesTransitionSetupDataSO* self, LevelCompletionResults* levelCompletionResults) {
        StandardLevelScenesTransitionSetupDataSO_Finish(self, levelCompletionResults);

        PresenterManager::DespawnforAll(PresenterManager::IN_SONG);
        PresenterManager::SpawnInMenu();
    }

    void PresenterHooks() {
        INSTALL_HOOK(getLogger(), AudioTimeSyncController_StartSong);
        INSTALL_HOOK(getLogger(), PauseMenuManager_ShowMenu);
        INSTALL_HOOK(getLogger(), PauseMenuManager_MenuButtonPressed);
        INSTALL_HOOK(getLogger(), PauseMenuManager_RestartButtonPressed);
        INSTALL_HOOK(getLogger(), PauseAnimationFinish);
        INSTALL_HOOK(getLogger(), StandardLevelScenesTransitionSetupDataSO_Finish);
    }
}