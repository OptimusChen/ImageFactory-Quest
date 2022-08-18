#pragma once

#include "HMUI/ViewController.hpp"
#include "UnityEngine/Transform.hpp"
#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"

#include "GlobalNamespace/LoadingControl.hpp"

DECLARE_CLASS_CODEGEN(ImageFactory::UI, ImageCreationViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::LoadingControl*, loadingControl);

    private:
        custom_types::Helpers::Coroutine SetupListElements(UnityEngine::Transform* parent);
);