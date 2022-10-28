#include "../include/PluginConfig.hpp"

#include "beatsaber-hook/shared/rapidjson/include/rapidjson/writer.h"
#include "UI/ImageFactoryFlowCoordinator.hpp"
#include "UI/ImageEditingViewController.hpp"
#include "Presenters/PresenterManager.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "UnityEngine/GameObject.hpp"
#include "Utils/StringUtils.hpp"
#include "Utils/FileUtils.hpp"
#include "IFImage.hpp"

DEFINE_CONFIG(PluginConfig);

using namespace UnityEngine;

namespace ImageFactory {
    void Config::Reset() {
        PluginConfig_t& pluginConfig = getPluginConfig();
        ConfigDocument& config = pluginConfig.config->config;
        std::vector<IFImage*> info;

        for (std::pair<IFImage*, std::string> pair : *PresenterManager::MAP) {
            Delete(pair.first, false);

            info.push_back(pair.first);
        }

        for (int i = 0; i < info.size(); i++) {
            PresenterManager::ClearInfo(info[i]);
        }

        pluginConfig.Amount.SetValue(0);
        pluginConfig.AnimateImages.SetValue(true);
        pluginConfig.Enabled.SetValue(true);
        pluginConfig.IgnoreNoTextAndHud.SetValue(false);
        pluginConfig.Images.SetValue("");
        pluginConfig.config->Write();
        pluginConfig.config->Reload();

        auto flow = Object::FindObjectsOfType<UI::ImageFactoryFlowCoordinator*>().First();
        flow->imageEditingViewController->ClearList();
    }   

    void Config::Add(IFImage* image) {
        PluginConfig_t& pluginConfig = getPluginConfig();
        ConfigDocument& configDoc = pluginConfig.config->config;
        rapidjson::Document::AllocatorType& allocator = pluginConfig.config->config.GetAllocator();
        rapidjson::Value configObj;

        configObj.SetObject();
        configObj.AddMember("x", image->x, allocator);
        configObj.AddMember("y", image->y, allocator);
        configObj.AddMember("z", image->z, allocator);
        configObj.AddMember("angleX", image->screen->get_transform()->get_rotation().get_eulerAngles().x, allocator);
        configObj.AddMember("angleY", image->screen->get_transform()->get_rotation().get_eulerAngles().y, allocator);
        configObj.AddMember("angleZ", image->screen->get_transform()->get_rotation().get_eulerAngles().z, allocator);
        configObj.AddMember("scaleX", image->scaleX, allocator);
        configObj.AddMember("scaleY", image->scaleY, allocator);
        configObj.AddMember("width", image->width, allocator);
        configObj.AddMember("height", image->height, allocator);
        configObj.AddMember("enabled", image->enabled, allocator);
        configObj.AddMember("path", image->path, allocator);
        configObj.AddMember("name", image->name, allocator);
        configObj.AddMember("presentationOption", image->presentationoption, allocator);

        std::string extraData;
        for (std::pair<StringW, StringW> pair : *image->extraData) {
            extraData = extraData + static_cast<std::string>(pair.first) + ";" + static_cast<std::string>(pair.second) + "/";
        }

        configObj.AddMember("extraData", extraData, allocator);
        configDoc.AddMember(rapidjson::Value(image->fileName + "_" + 
        std::to_string(pluginConfig.Amount.GetValue() + 1), allocator).Move(), configObj, allocator);
        
        pluginConfig.config->Write();
        pluginConfig.config->Reload();

        StringW s = pluginConfig.Images.GetValue();
        pluginConfig.Images.SetValue(s + "/" + image->fileName + "_" + std::to_string(pluginConfig.Amount.GetValue() + 1));
        pluginConfig.Amount.SetValue(pluginConfig.Amount.GetValue() + 1);
        image->internalName = image->fileName + "_" + std::to_string(pluginConfig.Amount.GetValue());
    }

    void Config::Update(IFImage* image) {
        PluginConfig_t& pluginConfig = getPluginConfig();
        ConfigDocument& configDoc = pluginConfig.config->config;
        if (configDoc.HasMember(image->internalName)) {    
            rapidjson::Document::AllocatorType& allocator = pluginConfig.config->config.GetAllocator();
            rapidjson::Value& configValue = configDoc[static_cast<std::string>(image->internalName)];

            configValue["x"].SetFloat(image->x);
            configValue["y"].SetFloat(image->y);
            configValue["z"].SetFloat(image->z);
            configValue["angleX"].SetFloat(image->angleX);
            configValue["angleY"].SetFloat(image->angleY);
            configValue["angleZ"].SetFloat(image->angleZ);
            configValue["scaleX"].SetFloat(image->scaleX);
            configValue["scaleY"].SetFloat(image->scaleY);
            configValue["width"].SetFloat(image->width);
            configValue["height"].SetFloat(image->height);
            configValue["enabled"].SetBool(image->enabled);
            configValue["path"].SetString(image->path.c_str(), allocator);
            configValue["name"].SetString(image->name.c_str(), allocator);
            configValue["presentationOption"].SetString(image->presentationoption.c_str(), allocator);

            std::string extraData;
            for (std::pair<StringW, StringW> pair : *image->extraData) {
                extraData = extraData + static_cast<std::string>(pair.first) + ";" + static_cast<std::string>(pair.second) + "/";
            }

            configValue["extraData"].SetString(extraData.c_str(), allocator);

            pluginConfig.config->Write();
            pluginConfig.config->Reload();
        }
    }

    void Config::Delete(IFImage* image, bool clearInfo) {
        PluginConfig_t& pluginConfig = getPluginConfig();
        ConfigDocument& configDoc = pluginConfig.config->config;
        if (configDoc.HasMember(image->internalName)) {
            configDoc.RemoveMember(image->internalName);
            pluginConfig.Amount.SetValue(pluginConfig.Amount.GetValue() - 1);
            
            StringW images = pluginConfig.Images.GetValue();

            pluginConfig.Images.SetValue(static_cast<std::string>(images->Replace("/" + image->internalName, Il2CppString::_get_Empty())));

            pluginConfig.config->Write();
            pluginConfig.config->Reload();

            image->Destroy();

            if (clearInfo) {
                PresenterManager::ClearInfo(image);
            }
        }
    }

    custom_types::Helpers::Coroutine Config::LoadImages() {
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.5f)));

        std::string images = getPluginConfig().Images.GetValue();
        ConfigDocument& config = getPluginConfig().config->config;

        std::vector<std::string> split = StringUtils::split(images, '/');

        for (int i = 0; i < split.size(); i++) {
            StringW fileName = split.at(i);

            if (fileName->get_Length() != 0) {  
                if (config.HasMember(fileName)) {
                    rapidjson::Value& configValue = config[static_cast<std::string>(fileName)];
                    GameObject* obj = GameObject::New_ctor(fileName);
                    GameObject::DontDestroyOnLoad(obj);
                    IFImage* image = obj->AddComponent<IFImage*>();
                    
                    image->path = configValue["path"].GetString();
                    image->sprite = BeatSaberUI::FileToSprite(image->path);
                    image->internalName = fileName;
                    image->x = configValue["x"].GetFloat();
                    image->y = configValue["y"].GetFloat();
                    image->z = configValue["z"].GetFloat();
                    image->angleX = configValue["angleX"].GetFloat();
                    image->angleY = configValue["angleY"].GetFloat();
                    image->angleZ = configValue["angleZ"].GetFloat();
                    image->scaleX = configValue["scaleX"].GetFloat();
                    image->scaleY = configValue["scaleY"].GetFloat();
                    image->width = configValue["width"].GetFloat();
                    image->height = configValue["height"].GetFloat();
                    image->name = configValue["name"].GetString();
                    image->presentationoption = configValue["presentationOption"].GetString();
                    image->enabled = configValue["enabled"].GetBool();
                    image->extraData = new std::unordered_map<std::string, std::string>();
                    image->spriteRenderer = image->get_gameObject()->AddComponent<SpriteRenderer*>();
                    image->isAnimated = FileUtils::isGifFile(image->path);
                    image->canAnimate = false;

                    getLogger().info("SPRITERENDERER: %p", image->spriteRenderer);

                    StringW extraData = configValue["extraData"].GetString();

                    if (extraData->get_Length() != 0) {
                        std::vector<std::string> pairs = StringUtils::split(extraData, '/');

                        for (int j = 0; j < pairs.size(); j++) {
                            StringW pair = pairs.at(j);

                            if (pair->get_Length() != 0) {
                                StringW key = StringUtils::split(pair, ';').at(0);
                                StringW val = StringUtils::split(pair, ';').at(1);
                                
                                image->SetExtraData(key, val);
                            }
                        }
                    }

                    image->fileName = FileUtils::GetFileName(image->path, false);
                    PresenterManager::Parse(image, image->presentationoption);

                    if (FileUtils::isGifFile(image->path)) {
                        image->sprite = UIUtils::FirstFrame(image->path);
                    }

                    image->spriteRenderer->set_sprite(UIUtils::FirstFrame(image->path));
                    image->spriteRenderer->get_gameObject()->SetActive(false);

                    image->Create();
                    image->Despawn(false);

                    bool finished = false;

                    StartCoroutine(image->SetImage([&](){
                        finished = true;
                    }));

                    while (!finished) {
                        co_yield nullptr;
                    }
                }
            }

            co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.05f)));
        }

        PresenterManager::SpawnInMenu();

        getLogger().info("DONE");

        co_return;
    }
}