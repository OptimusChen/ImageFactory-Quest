#pragma once

#include "Presenter.hpp"

namespace ImageFactory::Presenters {
    class ComboPresenter : public Presenter {
        public:
            std::vector<GameObject*> GetUIElements(Transform* parent, IFImage* image) override;
    };

    class ComboIncrementPresenter : public Presenter {
        public:
            std::vector<GameObject*> GetUIElements(Transform* parent, IFImage* image) override;
    };

    class ComboHoldPresenter : public Presenter {
        public:
            std::vector<GameObject*> GetUIElements(Transform* parent, IFImage* image) override;
    };

    class ComboDropPresenter : public Presenter {
        public:
            std::vector<GameObject*> GetUIElements(Transform* parent, IFImage* image) override;
    };
}