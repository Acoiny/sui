#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <sui/Colors.hpp>
#include <sui/baseElement.hpp>
#include <sui/basic/button.hpp>

using namespace sui;

Button::Button(float x, float y, float w, float h) : m_rect{x, y, w, h} {}

void Button::draw(SDL_Renderer *renderer)
{
  SDL_Color col{};

  bool render_outline = false;

  switch (m_state)
  {
  case ButtonState::IDLE:
    col = Color::BUTTON_BACKGROUND;
    break;
  case ButtonState::MOUSE_HOVER:
    col = Color::BUTTON_HOVER;
    render_outline = true;
    break;
  case ButtonState::MOUSE_HELD:
    col = Color::BUTTON_DOWN;
    render_outline = true;
    break;
  default:
    break;
  }

  auto [r, g, b, a] = col;
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderFillRect(renderer, &m_rect);

  if (render_outline)
  {
    auto [r, g, b, a] = Color::OUTLINE_COLOR;
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderRect(renderer, &m_rect);
  }
}

bool Button::HandleMouseEvent(SDL_Event &event)
{
  bool handled = false;
  switch (event.type)
  {
  case SDL_EVENT_MOUSE_MOTION:
  {
    float mx = event.motion.x, my = event.motion.y;
    bool intersects = isIntersecting(mx, my);

    // if (intersects)
    // {
    //   if (m_state == ButtonState::IDLE)
    //     m_state = ButtonState::MOUSE_HOVER;
    // }
    // // if the mouse is not over this button AND it's state is not idle,
    // // we set it's state but also say, that the event has not been handled.
    // else if (m_state != ButtonState::IDLE)
    // {
    //   m_state = ButtonState::IDLE;
    // }

    // mouse entered
    if (intersects && m_state == ButtonState::IDLE)
    {
      m_state = ButtonState::MOUSE_HOVER;

      handled = true;
    }
    // mouse is not over button
    else if (!intersects && m_state == ButtonState::MOUSE_HOVER)
    {
      m_state = ButtonState::IDLE;
      handled = true;
    }
    break;
  }
  case SDL_EVENT_MOUSE_BUTTON_DOWN:
  {
    if (m_state == ButtonState::MOUSE_HOVER)
    {
      m_state = ButtonState::MOUSE_HELD;
      handled = true;
    }
    break;
  }
  case SDL_EVENT_MOUSE_BUTTON_UP:
  {
    if (m_state == ButtonState::MOUSE_HELD)
    {
      float mx = event.motion.x, my = event.motion.y;
      bool intersects = isIntersecting(mx, my);

      if (intersects)
      {
        m_state = ButtonState::MOUSE_HOVER;
        if (onLeftClick)
          onLeftClick();
      }
      else
      {
        m_state = ButtonState::IDLE;
      }
      handled = true;
    }
    break;
  }
  }

  return handled;
}

void Button::HandleResizeEvent(const SDL_FRect &space)
{
  m_rect = space;
  applyMargin(m_rect);
}
