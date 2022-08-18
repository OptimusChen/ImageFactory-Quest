#pragma once

#include "HMUI/ViewController.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "GlobalNamespace/LoadingControl.hpp"

DECLARE_CLASS_CODEGEN(ImageFactory::UI, ImageEditingViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::LoadingControl*, loadingControl);
    DECLARE_INSTANCE_METHOD(void, Refresh);

    private:
        custom_types::Helpers::Coroutine SetupListElements(UnityEngine::Transform* parent);
);