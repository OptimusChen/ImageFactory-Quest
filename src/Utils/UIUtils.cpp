#include "Utils/UIUtils.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "System/Collections/Generic/Dictionary_2.hpp"
#include "GlobalNamespace/PlayerSpecificSettings.hpp"
#include "GlobalNamespace/PlayerDataModel.hpp"
#include "GlobalNamespace/PlayerData.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/RawImage.hpp"
#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/TextureWrapMode.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "HMUI/ImageView.hpp"
#include "Helpers/utilities.hpp"
#include "BSML/Animations/AnimationInfo.hpp"
#include "gif-lib/shared/gif_lib.h"
#include "EasyGifReader.h"
#include "PluginConfig.hpp"
#include "Sprites.hpp"
#include "main.hpp"
#include <map>

using namespace HMUI;
using namespace TMPro;
using namespace QuestUI;;
using namespace UnityEngine;
using namespace UnityEngine::UI;

#define SetPreferredSize(identifier, width, height)                                         \
    auto layout##identifier = identifier->get_gameObject()->GetComponent<LayoutElement*>(); \
    if (!layout##identifier)                                                                \
        layout##identifier = identifier->get_gameObject()->AddComponent<LayoutElement*>();  \
    layout##identifier->set_preferredWidth(width);                                          \
    layout##identifier->set_preferredHeight(height)

namespace UIUtils {

    Backgroundable* CreateHeader(Transform* transform, std::string name, Vector3 localPos, Color color, int width, int height) {
        GameObject* mainLayout = GameObject::New_ctor();
        RectTransform* parent = mainLayout->AddComponent<RectTransform*>();
        parent->SetParent(transform, false);
        parent->set_localPosition(localPos);

        VerticalLayoutGroup* vertical = BeatSaberUI::CreateVerticalLayoutGroup(parent);
        vertical->get_rectTransform()->set_anchoredPosition({40.0f, 35.0f});
        HorizontalLayoutGroup* horizontal =
            BeatSaberUI::CreateHorizontalLayoutGroup(vertical->get_transform());

        auto layoutElem = horizontal->GetComponent<LayoutElement*>();
        layoutElem->set_preferredHeight(height);
        layoutElem->set_preferredWidth(width);

        Backgroundable* background =
            horizontal->get_gameObject()->AddComponent<Backgroundable*>();
        background->ApplyBackgroundWithAlpha("round-rect-panel", color.a);

        ImageView* imageView =
            background->get_gameObject()->GetComponentInChildren<ImageView*>();
        imageView->gradient = true;
        imageView->gradientDirection = 0;
        imageView->set_color(UnityEngine::Color::get_white());
        imageView->set_color0(color);
        imageView->set_color1(color);
        imageView->curvedCanvasSettingsHelper->Reset();

        HorizontalLayoutGroup* header = horizontal;

        TextMeshProUGUI* text = BeatSaberUI::CreateText(header->get_transform(), name);
        text->set_fontSize(text->get_fontSize() * 1.5f);
        text->set_alignment(TextAlignmentOptions::Center);

        auto containerGroup = BeatSaberUI::CreateHorizontalLayoutGroup(transform);
        containerGroup->get_rectTransform()->set_anchoredPosition({0.0f, 0.0f});
        
        SetPreferredSize(containerGroup, 20, 80);

        auto bg = containerGroup->get_gameObject()->AddComponent<Backgroundable*>();
        bg->ApplyBackground(il2cpp_utils::newcsstr("round-rect-panel"));
        bg->background->set_color(color);

        return bg;
    }

    Backgroundable* CreateHeader(Transform* transform, std::string name) {
        return CreateHeader(transform, name, {38.0f, 0.0f, 0.0f}, Color(0.1f, 0.1f, 0.1f, 0.2f), 100, 10);
    }

    HMUI::ModalView* CreateModal(Sprite* sprite, std::string desc, Transform* parent, std::string name, std::string buttonText, std::function<void(HMUI::ModalView*)> onClick) {
        HMUI::ModalView* modal = BeatSaberUI::CreateModal(parent, {90.0f, 50.0f}, nullptr, true);
        GameObject* scrollableModal = BeatSaberUI::CreateScrollableModalContainer(modal);
         

        modal->Show(true, true, nullptr);

        if (name.compare("Help and FAQ") == 0) {
            BeatSaberUI::CreateImage(modal->get_transform(), sprite, {-38.0f, 19.0f}, {6.0f, 9.0f});
        } else {
            BeatSaberUI::CreateImage(modal->get_transform(), sprite, {-38.0f, 19.0f}, {9.0f, 9.0f});
        }

        BeatSaberUI::CreateUIButton(modal->get_transform(), "DISMISS", {-22.0f, -19.0f}, {40.0f, 8.0f}, [modal]() { modal->Hide(true, nullptr); });
        BeatSaberUI::CreateUIButton(modal->get_transform(), buttonText, {22.0f, -19.0f}, {40.0f, 8.0f}, [onClick, modal]() { onClick(modal); });
    
        BeatSaberUI::CreateImage(modal->get_transform(), BeatSaberUI::Base64ToSprite(Pixel), {0.0f, 13.0f}, {85.0f, 0.8f});
        BeatSaberUI::CreateText(modal->get_transform(), name, {-32.0f, 25.0f}, true)->set_fontSize(10.0f);
        BeatSaberUI::CreateText(modal->get_transform(), desc, {-42.0f, 10.0f}, true);

         return modal;
    }

    using namespace BSML;

    inline uint32_t make_black_transparent(const uint32_t& v) {
        return v >> 8 ? v : 0;
    }

    SafePtr<System::Collections::Generic::Dictionary_2<StringW, UnityEngine::Sprite*>> cache;
    Sprite* FirstFrame(std::string path) {
        if (!cache) cache.emplace(System::Collections::Generic::Dictionary_2<StringW, UnityEngine::Sprite*>::New_ctor());

        UnityEngine::Sprite* sprite = nullptr;
        if (cache->TryGetValue(path, byref(sprite)) && sprite && sprite->m_CachedPtr.m_value) {
            return sprite;
        } else {
            cache->Remove(path);
        }

        try {
            auto gifReader = EasyGifReader::openFile(path.c_str());
            int  width = gifReader.width(), height = gifReader.height(), frameCount = gifReader.frameCount();

            for (const auto& gifFrame : gifReader) {
                auto currentFrame = new FrameInfo(gifFrame.width(), gifFrame.height());

                const uint8_t* pixels = (const uint8_t*)gifFrame.pixels();
                uint8_t* colorData = currentFrame->colors.ptr()->values + currentFrame->colors.ptr()->Length();

                int height = gifFrame.height();
                int rowSize = sizeof(uint32_t) * gifFrame.width();
                
                for (int y = 0; y < height; y++) {
                    colorData -= rowSize;
                    
                    std::transform((uint32_t*)pixels, (uint32_t*)(pixels + rowSize), (uint32_t*)colorData, make_black_transparent);
                    pixels += rowSize;
                }

                auto frameTexture = Texture2D::New_ctor(gifFrame.width(), gifFrame.height(), UnityEngine::TextureFormat::RGBA32, false);
                frameTexture->set_wrapMode(TextureWrapMode::Clamp);
                frameTexture->LoadRawTextureData(currentFrame->colors.ptr());
                frameTexture->Apply();

                sprite = BSML::Utilities::LoadSpriteFromTexture(frameTexture);
                
                cache->Add(path, sprite);

                return sprite;
            }
        } catch (EasyGifReader::Error gifError) { }

        return nullptr;
    }

    GlobalNamespace::PlayerDataModel* data = nullptr;
    bool NoHud() {
        if (!data) data = Object::FindObjectOfType<GlobalNamespace::PlayerDataModel*>();

        bool ignore = getPluginConfig().IgnoreNoTextAndHud.GetValue();

        if (ignore) {
            return true;
        }

        return ignore == data->get_playerData()->get_playerSpecificSettings()->get_noTextsAndHuds();
    }
}