#pragma once

#include "IFImage.hpp"
#include "Presenters/Presenter.hpp"
#include "HMUI/ViewController.hpp"
#include "UnityEngine/Coroutine.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "custom-types/shared/macros.hpp"
#include "main.hpp"

DECLARE_CLASS_CODEGEN(ImageFactory::UI, ImageCreatorViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_FIELD(bool, hasSaved); 
    DECLARE_INSTANCE_FIELD(bool, editing); 
    DECLARE_INSTANCE_FIELD(IFImage*, image);
    DECLARE_INSTANCE_FIELD(IFImage*, backUpImage);
    DECLARE_INSTANCE_METHOD(void, InitializeEditor, IFImage*);

    public: 
        StringW path;
        std::vector<UnityEngine::GameObject*> options;
        Presenters::Presenter* presenter;
        
        void Initialize(StringW str);
        void ResetOptions(UnityEngine::Transform* parent);
        custom_types::Helpers::Coroutine UpdateImage();
    );