#include "../include/PluginConfig.hpp"

#include "Presentors/PresentorManager.hpp"

DEFINE_CONFIG(PluginConfig);

namespace ImageFactory {
    void Config::Reset() {
        
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

    void Config::Delete(IFImage* image) {
        PluginConfig_t& pluginConfig = getPluginConfig();
        ConfigDocument& configDoc = pluginConfig.config->config;
        if (configDoc.HasMember(image->internalName)) {
            configDoc.RemoveMember(image->internalName);
            pluginConfig.Amount.SetValue(pluginConfig.Amount.GetValue() - 1);
            
            std::string images = pluginConfig.Images.GetValue();

            pluginConfig.Images.SetValue(images.replace(images.begin(), images.end(), "/" + image->internalName));

            pluginConfig.config->Write();
            pluginConfig.config->Reload();

            image->Despawn();
        }
    }

    void Config::LoadImages() {
        
    }
}