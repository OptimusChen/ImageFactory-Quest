#include "IFImage.hpp"

#include "main.hpp"
#include "Sprites.hpp"
#include "Utils/FileUtils.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Rect.hpp"
#include "questui/shared/BeatSaberUI.hpp"

using namespace UnityEngine;
using namespace QuestUI;

DEFINE_TYPE(ImageFactory, IFImage);

namespace ImageFactory {

    void IFImage::Create() {
        if (hasBeenCreated) return;

        screen = BeatSaberUI::CreateFloatingScreen({scaleX * (width / 3), scaleY * (height / 3)}, {x, y, z}, {angleX, angleY, angleZ}, 0.0f, false, true, 4);
        image = BeatSaberUI::CreateImage(screen->get_transform(), sprite, {x, y}, {scaleX * (width / 3), scaleY * (height / 3)});
        UnityEngine::Object::DontDestroyOnLoad(screen);
        UnityEngine::Object::DontDestroyOnLoad(image);

        screen->set_active(false);

        hasBeenCreated = true;
    }

    void IFImage::Spawn() {
        if (!enabled) return; 
        if (!screen) return;

        screen->set_active(true);
    }

    void IFImage::Despawn() {
        if (!screen) return;
        
        screen->set_active(false);
    }

    void IFImage::Update(bool handle) {
        if (!enabled) {
            Despawn();
            return;
        }

        Vector3 oldPos = {x, y, z};
        Vector3 oldRot = {angleX, angleY, angleZ};

        if (screen) {
            oldPos = screen->get_transform()->get_localPosition();
            oldRot = screen->get_transform()->get_eulerAngles();
        }

        Object::Destroy(screen);
        Object::Destroy(image);

        screen = BeatSaberUI::CreateFloatingScreen({scaleX * (width / 3), scaleY * (height / 3)}, oldPos, oldRot, 0.0f, false, handle, 4);
        image = BeatSaberUI::CreateImage(screen->get_transform(), sprite, {x, y}, {scaleX * (width / 3), scaleY * (height / 3)});
        Object::DontDestroyOnLoad(screen);
        Object::DontDestroyOnLoad(image);

        screen->SetActive(enabled);
        image->get_gameObject()->SetActive(enabled);
    }

    void IFImage::Destroy() {
        hasBeenCreated = false;
        Object::Destroy(screen);
        Object::Destroy(image);
    }

    void IFImage::SetExtraData(StringW key, StringW val) {
        if (extraData->contains(key)) {
            extraData->erase(key);
        }
        extraData->insert({key, val});
    }

    std::string IFImage::GetExtraData(StringW key, StringW defaultVal) {
    if (extraData->contains(key)) {
        return static_cast<std::string>(extraData->at(key));
    } else {
        return static_cast<std::string>(defaultVal);
    }
    }

    void IFImage::ctor(UnityEngine::Sprite* s, StringW p) {
        sprite = s;
        width = sprite->get_textureRect().get_width();
        height = sprite->get_textureRect().get_height();
        x = 0.0f;
        y = 3.0f;
        z = 5.0f;
        name = FileUtils::GetFileName(p, false);
        angleX = 0;
        angleY = 0;
        angleZ = 0;
        enabled = true;
        scaleX = 1.0f;
        scaleY = 1.0f;
        presentationoption = "Everywhere";
        inSong = false;
        fileName = FileUtils::GetFileName(p, false);
        path = static_cast<std::string>(p);
        extraData = new std::unordered_map<std::string, std::string>();

        Create();
        Update(true);
    }
}

