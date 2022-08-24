#pragma once

#include "IFImage.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "main.hpp"

#include "custom-types/shared/coroutine.hpp"
#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(PluginConfig, 
    CONFIG_VALUE(Images, std::string, "Saved Images", "");
    CONFIG_VALUE(Amount, int, "Images", 0);
    CONFIG_VALUE(Enabled, bool, "Enabled", true);
    CONFIG_VALUE(AnimateImages, bool, "AnimateImages", true);
    CONFIG_VALUE(IgnoreNoTextAndHud, bool, "IgnoreNoTextAndHud", false);
    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(Images); 
        CONFIG_INIT_VALUE(Amount);
        CONFIG_INIT_VALUE(Enabled);
        CONFIG_INIT_VALUE(AnimateImages);
        CONFIG_INIT_VALUE(IgnoreNoTextAndHud);
));

namespace ImageFactory {
    class Config {
        public:
            static void Reset();
            static void Delete(IFImage* image);
            static void Add(IFImage* image);
            static void Update(IFImage* image);
            static custom_types::Helpers::Coroutine LoadImages();
    };
}