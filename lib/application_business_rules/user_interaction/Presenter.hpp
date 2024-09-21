#pragma once
#include "IPresenter.hpp"
#include "IStatusIndicator.hpp"
#include "Menu.hpp"
#include <vector>

class Presenter : public IPresenter
{
  public:
    Presenter(Menu &, const std::vector<IStatusIndicator *> &);
    void setTaskStatusIndicator(const TaskIndex, const TaskIndicatorState) override;
    void loop();

  private:
    Menu &menu;
    const std::vector<IStatusIndicator *> statusIndicators;
};
