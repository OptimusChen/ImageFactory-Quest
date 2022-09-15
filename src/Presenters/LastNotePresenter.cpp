#include "main.hpp"

#include "Utils/StringUtils.hpp"
#include "Presenters/PresenterManager.hpp"
#include "Presenters/LastNotePresenter.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "GlobalNamespace/StandardLevelDetailView.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/BeatmapData.hpp"
#include "GlobalNamespace/BeatmapDataItem.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "System/Threading/Tasks/Task_1.hpp"
#include "System/Collections/Generic/LinkedList_1.hpp"
#include "GlobalNamespace/NoteController.hpp"

using namespace QuestUI;

#define StartCoroutine(method) GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(method))

namespace ImageFactory::Presenters {

    std::vector<GameObject*> LastNotePresenter::GetUIElements(Transform* parent, IFImage* image) {
        std::vector<GameObject*> ret;

        auto duration = BeatSaberUI::CreateIncrementSetting(parent, "Duration (Seconds)", 2, 0.25, std::stof(image->GetExtraData("last_note_duration", "1")), 0.25, 100, 
            [=](float f){
                image->SetExtraData("last_note_duration", StringUtils::removeTrailingZeros(round(f)));
            })->get_gameObject();

        ret.push_back(duration);

        return ret;
    }

    custom_types::Helpers::Coroutine GetNotesCount(IDifficultyBeatmap* difficultyBeatmap, std::function<void(int)> callback) {
        IPreviewBeatmapLevel* level = reinterpret_cast<IPreviewBeatmapLevel*>(difficultyBeatmap->get_level());
        
        auto envInfo = level->get_environmentInfo();
        auto task = difficultyBeatmap->GetBeatmapDataAsync(envInfo, nullptr);

        while (!task->get_IsCompleted()) co_yield nullptr;

        auto data = task->get_ResultOnSuccess();
        auto list = List<NoteData*>::New_ctor();
        list->AddRange(data->GetBeatmapDataItems<NoteData*>());
        
        callback(list->get_Count());
    }

    int noteCount = 0;

    MAKE_HOOK_MATCH(StandardLevelDetailView_RefreshContent, &StandardLevelDetailView::RefreshContent, void, StandardLevelDetailView* self) {
        StandardLevelDetailView_RefreshContent(self);

        StartCoroutine(GetNotesCount(self->selectedDifficultyBeatmap, [&](int notes){
            noteCount = notes;
        }));
    }

    MAKE_HOOK_MATCH(NoteController_Init, &NoteController::Init, void, NoteController* self, NoteData* noteData, float worldRotation, Vector3 moveStartPos, Vector3 moveEndPos, Vector3 jumpEndPos, float moveDuration, float jumpDuration, float jumpGravity, float endRotation, float uniformScale, bool rotateTowardsPlayer, bool useRandomRotation) {
        Presenter::currentNoteCount++;

        if (!UIUtils::NoHud()) return;

        if (Presenter::currentNoteCount == noteCount) {
            for (std::pair<IFImage*, StringW> pair : *PresenterManager::MAP) {
                IFImage* image = pair.first;
                if (pair.second.starts_with(PresenterManager::LAST_NOTE)) {
                    StartCoroutine(PresenterManager::DespawnAfter(image, stof(image->GetExtraData("last_note_duration", "1"))));
                }
            }
        }
         
        NoteController_Init(self, noteData, worldRotation, moveStartPos, moveEndPos,
                            jumpEndPos, moveDuration, jumpDuration, jumpGravity,
                            endRotation, uniformScale, rotateTowardsPlayer, useRandomRotation);
    }

    void LastNoteHooks() {
        INSTALL_HOOK(getLogger(), StandardLevelDetailView_RefreshContent);
        INSTALL_HOOK(getLogger(), NoteController_Init);
    }
}