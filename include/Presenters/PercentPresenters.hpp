#pragma once

#include "Presenter.hpp"

namespace ImageFactory::Presenters {
    class PercentPresenter : public Presenter {
        public:
            std::vector<GameObject*> GetUIElements(Transform* parent, IFImage* image) override;
    };

    class PercentRangePresenter : public Presenter {
        public:
            std::vector<GameObject*> GetUIElements(Transform* parent, IFImage* image) override;
    };
}