#include "UI/ImageEditingViewController.hpp"

#include "UI/ImageFactoryFlowCoordinator.hpp"
#include "Utils/UIUtils.hpp"
#include "Utils/FileUtils.hpp"
#include "Utils/StringUtils.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/Touchable.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/ArrayUtil.hpp"
#include "Presentors/PresentorManager.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/typedefs-string.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "PluginConfig.hpp"
#include "main.hpp"

DEFINE_TYPE(ImageFactory::UI, ImageEditingViewController);

using namespace GlobalNamespace;
using namespace UnityEngine::UI;
using namespace UnityEngine;
using namespace QuestUI;
using namespace HMUI;

#define StartCoroutine(method) GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(method))

namespace ImageFactory::UI {

    void ImageEditingViewController::DidActivate(bool firstActivation,
                                              bool addedToHierarchy,
                                              bool screenSystemEnabling) {
        if (firstActivation) 
        {
            if (!get_gameObject()) return;

            get_gameObject()->AddComponent<Touchable*>();

            auto bg = UIUtils::CreateHeader(BeatSaberUI::CreateHorizontalLayoutGroup(get_transform())->get_transform(), "New Image");

            auto listBar = BeatSaberUI::CreateHorizontalLayoutGroup(get_transform());
        
            auto listBarElement = listBar->GetComponent<LayoutElement*>();
            listBarElement->set_preferredWidth(20);
            listBarElement->set_preferredHeight(60);

            auto listBG = listBar->get_gameObject()->AddComponent<Backgroundable*>();
            listBG->ApplyBackground(il2cpp_utils::newcsstr("panel-top"));
            listBG->background->set_color(UnityEngine::Color(0.1f, 0.1f, 0.1f, 0.1f));
    
            auto container = BeatSaberUI::CreateScrollableSettingsContainer(listBG->get_transform());

            StartCoroutine(SetupListElements(container->get_transform()));
        }
    }

    custom_types::Helpers::Coroutine ImageEditingViewController::SetupListElements(Transform* list){
        if (loadingControl) {
            loadingControl->refreshButton->get_gameObject()->SetActive(false);
            loadingControl->refreshText->get_gameObject()->SetActive(false);
        }
        
        GameObject::Destroy(loadingControl);
        
        GameObject* existingLoadinControl = Resources::FindObjectsOfTypeAll<LoadingControl*>()->values[0]->get_gameObject();
        GameObject* loadinControlGameObject = UnityEngine::GameObject::Instantiate(existingLoadinControl, get_transform());

        auto loadingControlTransform = loadinControlGameObject->get_transform();
        loadingControl = loadinControlGameObject->GetComponent<LoadingControl*>();
        loadingControl->get_gameObject()->SetActive(true);
        loadingControl->set_enabled(true);

        ConstString load = "Loading Images...";

        loadingControl->refreshText->set_text(load);
        loadingControl->loadingText->set_text(load);
        loadingControl->ShowLoading(load);

        list->get_gameObject()->set_active(false);

        StringW s = getPluginConfig().Images.GetValue();
        ConfigDocument& config = getPluginConfig().config->config;
        int i = 0;

        vector<std::string> split = StringUtils::split(static_cast<std::string>(s), '/');

        loadingControl->loadingText->set_text("Loading Images... (0/" + std::to_string(split.size()) + ")");

        getLogger().info("Images: %s", static_cast<std::string>(s).c_str());
        getLogger().info("Length: %lu", split.size());

        for (int j = 0; j < split.size(); j++) {
            getLogger().info("Split: %s", static_cast<std::string>(split[j]).c_str());
        }

        while (!(i == split.size())) {
            StringW fileName = split.at(i);

            if (fileName->get_Length() != 0) {
                HorizontalLayoutGroup* levelBarLayout = BeatSaberUI::CreateHorizontalLayoutGroup(list->get_transform());
                levelBarLayout->set_childControlWidth(false);

                GameObject* prefab = levelBarLayout->get_gameObject();

                LayoutElement* levelBarLayoutElement = levelBarLayout->GetComponent<LayoutElement*>();
                levelBarLayoutElement->set_minHeight(10.0f);
                levelBarLayoutElement->set_minWidth(20.0f);

                if (config.HasMember(fileName)) {
                    rapidjson::Value& configValue = config[static_cast<std::string>(fileName)];
                    Sprite* sprite = BeatSaberUI::FileToSprite(configValue["path"].GetString());
                    BeatSaberUI::CreateImage(levelBarLayoutElement->get_transform(), sprite, {2.0f, 0.0f}, {10.0f, 2.0f});
                    if (!configValue["enabled"].GetBool()) {
                        BeatSaberUI::CreateText(levelBarLayoutElement->get_transform(), configValue["name"].GetString(), true)->set_color(Color(1.0f, 0.0f, 0.0f, 1.0f));
                    } else {
                        BeatSaberUI::CreateText(levelBarLayoutElement->get_transform(), configValue["name"].GetString(), true)->set_color(Color(0.0f, 1.0f, 0.0f, 1.0f));
                    }

                    levelBarLayoutElement->set_minWidth(1.0f);
                    levelBarLayoutElement->set_minHeight(1.0f);

                    auto deleteButton = BeatSaberUI::CreateUIButton(levelBarLayoutElement->get_transform(), "", {0.0f, 0.0f}, {12.0f, 9.0f}, 
                        [=]() {
                            for (std::pair<IFImage*, std::string> pair : *PresentorManager::MAP) {
                                if (pair.first->internalName.starts_with(fileName)) {
                                    Config::Delete(pair.first);
                                    Refresh();
                                    ArrayUtil::First(Object::FindObjectsOfType<ImageFactoryFlowCoordinator*>())->ResetViews();
                                }
                            }
                        });

                    auto deleteText = BeatSaberUI::CreateText(
                        deleteButton->get_transform(), "X");
                    deleteText->set_alignment(TMPro::TextAlignmentOptions::Center);
                    deleteText->set_color(Color(1.0f, 0.0f, 0.0f, 1.0f));

                    auto editButton = BeatSaberUI::CreateUIButton(levelBarLayoutElement->get_transform(), "", {0.0f, 0.0f}, {12.0f, 9.0f}, 
                        [=]() {
                            for (std::pair<IFImage*, std::string> pair : *PresentorManager::MAP) {
                                if (pair.first->internalName.starts_with(fileName)) {
                                    // edit image
                                }
                            }
                        });
                    auto editText = BeatSaberUI::CreateText(editButton->get_transform(), "<-");
                    editText->set_alignment(TMPro::TextAlignmentOptions::Center);
                }
            }

            i++;

            loadingControl->loadingText->set_text("Loading Images... (" + std::to_string(i - 1) + "/" + std::to_string(split.size() - 1) + ")");

            co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.4)));
        }

        co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.15f)));

        if (i == 1) {
            loadingControl->refreshText->get_gameObject()->SetActive(true);
            loadingControl->ShowText("No Images Found in Config!", false);
        } else {
            loadingControl->Hide();
        }

        list->get_gameObject()->SetActive(true);
    }

    void ImageEditingViewController::Refresh() {
        Object::Destroy(this);

        ImageFactoryFlowCoordinator* flow = ArrayUtil::First(Object::FindObjectsOfType<ImageFactoryFlowCoordinator*>());
        flow->SetRightScreenViewController(BeatSaberUI::CreateViewController<ViewController*>(), HMUI::ViewController::AnimationType::In);

        flow->imageEditingViewController = reinterpret_cast<ImageEditingViewController*>(BeatSaberUI::CreateViewController<ImageEditingViewController*>());
        flow->SetRightScreenViewController(flow->imageEditingViewController, HMUI::ViewController::AnimationType::In);
    }
}
