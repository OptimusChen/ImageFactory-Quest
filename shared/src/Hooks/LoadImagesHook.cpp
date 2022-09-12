#include "main.hpp"

#include "Presenters/PresenterManager.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "PluginConfig.hpp"

#include "Helpers/utilities.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "Sprites.hpp"

using namespace ImageFactory;
using namespace UnityEngine;
using namespace GlobalNamespace;

#define StartCoroutine(method) GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(method))

MAKE_HOOK_MATCH(MainMenuViewController_DidActivate, &GlobalNamespace::MainMenuViewController::DidActivate, void, GlobalNamespace::MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    MainMenuViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if (firstActivation) {
        StartCoroutine(Config::LoadImages());
    }
}


void Hooks::LoadImagesHook() {
    INSTALL_HOOK(getLogger(), MainMenuViewController_DidActivate);
}