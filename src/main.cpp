#include "main.hpp"

#include "../include/PluginConfig.hpp"
#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "UI/ImageFactoryFlowCoordinator.hpp"

using namespace ImageFactory;

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

void ImageFactory::InstallHooks() {
    Hooks::LoadImagesHook();
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load();
    getPluginConfig().Init(info);
    getConfig().Reload();
    getConfig().Write();  // Load the config file
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    QuestUI::Init();
    custom_types::Register::AutoRegister();

    QuestUI::Register::RegisterMainMenuModSettingsFlowCoordinator<UI::ImageFactoryFlowCoordinator*>(modInfo);
    QuestUI::Register::RegisterModSettingsFlowCoordinator<UI::ImageFactoryFlowCoordinator*>(modInfo);

    getLogger().info("Installing hooks...");
    InstallHooks();
    getLogger().info("Installed all hooks!");
}