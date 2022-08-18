#include "main.hpp"

#include "GlobalNamespace/MainMenuViewController.hpp"

using namespace ImageFactory;
using namespace UnityEngine;
using namespace GlobalNamespace;

MAKE_HOOK_MATCH(MainMenuViewController_DidActivate, &GlobalNamespace::MainMenuViewController::DidActivate, void, GlobalNamespace::MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    MainMenuViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    
}


void Hooks::LoadImagesHook() {
    INSTALL_HOOK(getLogger(), MainMenuViewController_DidActivate);
}