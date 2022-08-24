#pragma once

#include "Presenter.hpp"

namespace ImageFactory::Presenters {
    class ResultsScreenPresenter : public Presenter {
        public:
            std::vector<GameObject*> GetUIElements(Transform* parent, IFImage* image) override;
    };
}