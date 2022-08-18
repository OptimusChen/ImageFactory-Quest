#include "UI/ImageFactoryFlowCoordinator.hpp"

#include "main.hpp"
#include "Presentors/PresentorManager.hpp"
#include "UI/ImageCreatorViewController.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"

DEFINE_TYPE(ImageFactory::UI, ImageFactoryFlowCoordinator);

using namespace HMUI;
using namespace QuestUI::BeatSaberUI;

namespace ImageFactory::UI {

    void ImageFactoryFlowCoordinator::DidActivate(bool firstActivation,
                                              bool addedToHierarchy,
                                              bool screenSystemEnabling) {
        if (firstActivation) {
            SetTitle("IMAGEFACTORY", ViewController::AnimationDirection::Horizontal);
            showBackButton = true;

            imageCreationViewController = reinterpret_cast<ImageCreationViewController*>(CreateViewController<ImageCreationViewController*>());
            imageEditingViewController = reinterpret_cast<ImageEditingViewController*>(CreateViewController<ImageEditingViewController*>());
            imageFactoryViewController = reinterpret_cast<ImageFactoryViewController*>(CreateViewController<ImageFactoryViewController*>());
        
            imageEditingViewController->Refresh();

            ProvideInitialViewControllers(imageFactoryViewController, imageCreationViewController, imageEditingViewController, nullptr, nullptr);
        }             

        SetRightScreenViewController(imageEditingViewController, HMUI::ViewController::AnimationType::In);
    }

    void ImageFactoryFlowCoordinator::CreateImage(StringW path) {
        ImageCreatorViewController* viewController = reinterpret_cast<ImageCreatorViewController*>(BeatSaberUI::CreateViewController<ImageCreatorViewController*>());

        if (!viewController) return;

        viewController->Initialize(path);

        SetLeftScreenViewController(BeatSaberUI::CreateViewController<HMUI::ViewController*>(), HMUI::ViewController::AnimationType::In);
        SetRightScreenViewController(BeatSaberUI::CreateViewController<HMUI::ViewController*>(), HMUI::ViewController::AnimationType::In);
        ReplaceTopViewController(viewController, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        
        PresentorManager::DespawnAll();
    }

    void ImageFactoryFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topView) {
        parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
    }

    void ImageFactoryFlowCoordinator::ResetViews() {
        ReplaceTopViewController(imageFactoryViewController, this, this, nullptr, HMUI::ViewController::AnimationType::In, HMUI::ViewController::AnimationDirection::Horizontal);
        SetLeftScreenViewController(imageCreationViewController, HMUI::ViewController::AnimationType::In);
        SetRightScreenViewController(imageEditingViewController, HMUI::ViewController::AnimationType::In);
    }
}

