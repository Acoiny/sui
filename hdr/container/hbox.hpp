#pragma once

#include "container/ibox.hpp"

namespace sui
{
class HBox : public IBox
{
public:
  HBox() = default;
  virtual ~HBox() = default;

  void HandleResizeEvent(const SDL_FRect &space) override;
};
} // namespace sui
