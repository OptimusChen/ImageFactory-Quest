#include "main.hpp"

#include "GlobalNamespace/MenuTransitionsHelper.hpp"
#include "Presenters/PresenterManager.hpp"

using namespace GlobalNamespace;

namespace ImageFactory {
    MAKE_HOOK_MATCH(MenuTransitionsHelper_RestartGame, &MenuTransitionsHelper::RestartGame, void, MenuTransitionsHelper* self, System::Action_1<Zenject::DiContainer *> *finishCallback) {
        for (std::pair<IFImage*, std::string> pair : *PresenterManager::MAP) {
            pair.first->Destroy();
        }

        PresenterManager::MAP->clear();

        MenuTransitionsHelper_RestartGame(self, finishCallback);
    }

    void Hooks::MenuTransitionsHelper() {
        INSTALL_HOOK(getLogger(), MenuTransitionsHelper_RestartGame);
    }
}

