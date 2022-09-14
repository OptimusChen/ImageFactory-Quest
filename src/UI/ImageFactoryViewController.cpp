#include "UI/ImageFactoryViewController.hpp"

#include "PluginConfig.hpp"
#include "Sprites.hpp"
#include "Utils/UIUtils.hpp"
#include "HMUI/Touchable.hpp"
#include "UnityEngine/Application.hpp"
#include "UnityEngine/Vector2.hpp"
#include "questui/shared/BeatSaberUI.hpp"

DEFINE_TYPE(ImageFactory::UI, ImageFactoryViewController);

using namespace UnityEngine::UI;
using namespace UnityEngine;
using namespace UIUtils;
using namespace QuestUI;
using namespace HMUI;

namespace ImageFactory::UI {

    void ImageFactoryViewController::DidActivate(bool firstActivation,
                                              bool addedToHierarchy,
                                              bool screenSystemEnabling) {
        if (firstActivation) 
        {
            if (!get_gameObject()) return;

            get_gameObject()->AddComponent<Touchable*>();

            UnityEngine::Sprite* logo = BeatSaberUI::Base64ToSprite(Logo);
            UnityEngine::Sprite* optimus = BeatSaberUI::Base64ToSprite(Optimus);
            UnityEngine::Sprite* bandoot = BeatSaberUI::Base64ToSprite(Bandoot);
            UnityEngine::Sprite* github = BeatSaberUI::Base64ToSprite(Github);
            UnityEngine::Sprite* help = BeatSaberUI::Base64ToSprite(Help);
            UnityEngine::Sprite* settings = BeatSaberUI::Base64ToSprite(Settings);
            UnityEngine::Sprite* reset = BeatSaberUI::Base64ToSprite(Reset);

            BeatSaberUI::CreateImage(this->get_transform(), logo, {0.0f, 0.0f}, {58.0f, 58.0f});

            auto sButton = BeatSaberUI::CreateClickableImage(this->get_transform(), settings, {-35.0f, 20.0f}, {18.0f, 18.0f}, [=](){
                HMUI::ModalView* modal = BeatSaberUI::CreateModal(this->get_transform(), UnityEngine::Vector2(76.0f, 28.0f), nullptr, true);

                GameObject* scrollableModal = BeatSaberUI::CreateScrollableModalContainer(modal);
                BeatSaberUI::CreateToggle(scrollableModal->get_transform(), "Enabled", getPluginConfig().Enabled.GetValue(),
                    [](bool b) { getPluginConfig().Enabled.SetValue(b); });

                BeatSaberUI::CreateToggle(scrollableModal->get_transform(), "Animate Images", getPluginConfig().AnimateImages.GetValue(),
                    [](bool b) { getPluginConfig().AnimateImages.SetValue(b); });

                BeatSaberUI::CreateToggle(scrollableModal->get_transform(), "Ignore No Text and HUDs", getPluginConfig().IgnoreNoTextAndHud.GetValue(),
                    [](bool b) { getPluginConfig().IgnoreNoTextAndHud.SetValue(b); });

                modal->Show(true, true, nullptr);
             });
            BeatSaberUI::AddHoverHint(sButton, "Global Mod Settings");

            auto rButton = BeatSaberUI::CreateClickableImage(this->get_transform(), reset, {-35.0f, 0.0f}, {18.0f, 18.0f}, [=](){ 
                Config::Reset();
            });

            rButton->set_preserveAspect(true);
            BeatSaberUI::AddHoverHint(rButton, "Reset All Config Data");

            auto hButton = BeatSaberUI::CreateClickableImage(this->get_transform(), help, {-35.0f, -20.0f}, {18.0f, 18.0f}, [=](){ 
                CreateModal(help, "ImageFactory allows you to place custom images throughout your\ngame. Everything has been designed to be easy for the user to "
                            "use\nand customize. To learn more about how to use this mod, you can\nwatch this video showcasing how to use it.", this->get_transform(), 
                            "Help and FAQ", "Youtube Link", [=](HMUI::ModalView* modal){
                                Application::OpenURL("https://www.youtube.com/watch?v=8WZ-Jvg-OOo");
                            });
            });
            hButton->set_preserveAspect(true);
            BeatSaberUI::AddHoverHint(hButton, "Help and Tutorial");

            auto oButton = BeatSaberUI::CreateClickableImage(this->get_transform(), optimus, {35.0f, -20.0f}, {18.0f, 18.0f}, [=](){
                CreateModal(optimus, "sum random guy named optimus ported this mod to quest", this->get_transform(), 
                            "Optimus", "Github Link", [=](HMUI::ModalView* modal){
                                Application::OpenURL("https://github.com/OptimusChen");
                            });
            });
            BeatSaberUI::AddHoverHint(oButton, "The mod creator!");

            auto bButton = BeatSaberUI::CreateClickableImage(this->get_transform(), bandoot, {35.0f, 0.0f}, {18.0f, 18.0f}, [=](){
                CreateModal(bandoot, "Bandoot commissioned for the original PC ImageFactory mod to\nbe made, go check him out!", this->get_transform(), 
                            "Bandoot", "Twitch Link", [=](HMUI::ModalView* modal){
                                Application::OpenURL("https://www.twitch.tv/bandoot");
                            });
            });
            BeatSaberUI::AddHoverHint(bButton, "The mod commissioner!");

            auto gButton = BeatSaberUI::CreateClickableImage(this->get_transform(), github, {35.0f, 20.0f}, {18.0f, 18.0f}, [=](){
                CreateModal(github, "ImageFactory is open source! You can view it on GitHub. Have a\nbug report or a feature request? Submit an issue on GitHub.", this->get_transform(), 
                            "Github", "Github Link", [=](HMUI::ModalView* modal){
                                Application::OpenURL("https://github.com/OptimusChen/ImageFactory-Quest");
                            });
            });
            BeatSaberUI::AddHoverHint(gButton, "View the Github Page!");
        }
    }
}
