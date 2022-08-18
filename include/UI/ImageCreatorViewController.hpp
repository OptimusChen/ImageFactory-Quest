#pragma once

#include "IFImage.hpp"
#include "HMUI/ViewController.hpp"
#include "UnityEngine/Coroutine.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "custom-types/shared/macros.hpp"
#include "main.hpp"

DECLARE_CLASS_CODEGEN(ImageFactory::UI, ImageCreatorViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_FIELD(bool, hasSaved); 
    DECLARE_INSTANCE_FIELD(IFImage*, image);

    public: 
        StringW path;
        void Initialize(StringW str);
        custom_types::Helpers::Coroutine UpdateImage();
    );