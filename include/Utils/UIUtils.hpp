#pragma once

#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "UnityEngine/EventSystems/PointerEventData.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "HMUI/ModalView.hpp"
#include <string_view>

using namespace QuestUI;
using namespace UnityEngine;

namespace UIUtils {
    Backgroundable* CreateHeader(Transform* parent, std::string name);
    Backgroundable* CreateHeader(Transform* parent, std::string name, Vector3 localPos, Color color, int width, int height);
    HMUI::ModalView* CreateModal(Sprite* sprite, std::string desc, Transform* parent, std::string name, std::string buttonText, std::function<void(HMUI::ModalView*)> onClick);
    Sprite* FirstFrame(std::string path);
}