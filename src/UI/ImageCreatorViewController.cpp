#include "UI/ImageCreatorViewController.hpp"

#include "UI/ImageFactoryFlowCoordinator.hpp"
#include "PluginConfig.hpp"
#include "Utils/UIUtils.hpp"
#include "Presentors/PresentorManager.hpp"
#include "HMUI/Touchable.hpp"
#include "questui/shared/BeatSaberUI.hpp"

using namespace UnityEngine;
using namespace QuestUI;
using namespace HMUI;

DEFINE_TYPE(ImageFactory::UI, ImageCreatorViewController);

namespace ImageFactory::UI {

    void ImageCreatorViewController::DidActivate(bool firstActivation,
                                         bool addedToHierarchy,
                                         bool screenSystemEnabling) {
        if (firstActivation) {
            if (!get_gameObject()) return;

            get_gameObject()->AddComponent<Touchable*>();

            image->Create();

            GameObject* container = BeatSaberUI::CreateScrollableSettingsContainer(this->get_transform());

            BeatSaberUI::CreateStringSetting(container->get_transform(), "Name", image->name,
                [=](StringW s) { 
                    image->name = static_cast<std::string>(s);
            });

            BeatSaberUI::CreateToggle(container->get_transform(), "Enabled", image->enabled, 
                [=](bool b) {
                    image->enabled = b;
                    image->Update(true);
            });

            BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Scale X", 2, 0.1f, image->scaleX,
                [=](float f) {
                    image->scaleX = f;
                    image->Update(true);
            });

            BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Scale Y", 2, 0.1f, image->scaleY,
                [=](float f) {
                    image->scaleY = f;
                    image->Update(true);
            });

            UIUtils::CreateHeader(container->get_transform(), "", Vector3::get_zero(), Color(0.0f, 0.0f, 0.0f, 0.8f), 90, 40);

            ImageFactoryFlowCoordinator* flow = ArrayUtil::First(Object::FindObjectsOfType<ImageFactoryFlowCoordinator*>());

            auto cancelButton = BeatSaberUI::CreateUIButton(this->get_transform(), "", Vector2(-22.0f, -38.0f), Vector2(40.0f, 8.0f), 
                [=]() {
                    PresentorManager::ClearInfo(image);
                    image->Despawn();
                    Object::Destroy(image);

                    flow->ResetViews();
            });

            BeatSaberUI::CreateText(cancelButton->get_transform(), "CANCEL")->set_alignment(TMPro::TextAlignmentOptions::Center);

            auto saveButton = BeatSaberUI::CreateUIButton(this->get_transform(), "", {22.0f, -38.0f}, {40.0f, 8.0f},
                [=]() {
                    GameObject* screen = image->screen;

                    image->x = screen->get_transform()->get_localPosition().x;
                    image->y = screen->get_transform()->get_localPosition().y;
                    image->z = screen->get_transform()->get_localPosition().z;
                    image->angleX = screen->get_transform()->get_rotation().get_eulerAngles().x;
                    image->angleY = screen->get_transform()->get_rotation().get_eulerAngles().y;
                    image->angleZ = screen->get_transform()->get_rotation().get_eulerAngles().z;

                    getLogger().info("Name %s", static_cast<std::string>(image->name).c_str());
                    getLogger().info("Pres Opt %s", static_cast<std::string>(image->presentationoption).c_str());
                    getLogger().info("Path %s", static_cast<std::string>(image->path).c_str());

                    Config::Add(image);

                    hasSaved = true;

                    image->Update(false);

                    flow->imageEditingViewController->Refresh();
                    flow->ResetViews();
            });

            BeatSaberUI::CreateText(saveButton->get_transform(), "SAVE")->set_alignment(TMPro::TextAlignmentOptions::Center);
        }
    }

    void ImageCreatorViewController::DidDeactivate(bool removedFromHierarchy,
                                           bool screenSystemEnabling) {
        if (image) {
            if (!hasSaved) {
                PresentorManager::ClearInfo(image);
                image->Despawn();
                Object::Destroy(image);
            }
        }
    }

    void ImageCreatorViewController::Initialize(StringW s) {
        path = s;

        GameObject* obj = GameObject::New_ctor(path);
        IFImage* image = obj->AddComponent<IFImage*>();
        image->ctor(BeatSaberUI::FileToSprite(static_cast<std::string>(s)), s);

        this->image = image;

        hasSaved = false;
    }
}