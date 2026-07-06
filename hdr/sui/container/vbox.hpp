#pragma once

#include <SDL3/SDL_rect.h>
#include <sui/container/ibox.hpp>

namespace sui
{
class VBox : public IBox
{
public:
  VBox() = default;
  virtual ~VBox() = default;

  void HandleResizeEvent(const SDL_FRect &space) override;
};
}; // namespace sui
