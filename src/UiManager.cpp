#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <sui/Colors.hpp>
#include <sui/UiManager.hpp>
#include <sui/config.hpp>
#include <sui/logger.hpp>
#include <sui/textManager.hpp>
#include <sui/themes.hpp>

using namespace sui;

UiManager::UiManager(SDL_Renderer *renderer, const std::string &app_name)
{
  BaseElement::s_uimanager = this;

  Config::set_application_name(app_name);

  // load default config first, then overwrite it with custom config file
  Color::LoadDefaultColorTheme();
  Theme::LoadFromConfig();

  TextManager::Init(renderer);
}

UiManager::~UiManager()
{
  m_elements.clear();
  TextManager::Deinit();
}

void UiManager::handleEvent(SDL_Event &event)
{
  if (event.type == SDL_EVENT_WINDOW_RESIZED)
  {
    auto x = event.window.data1;
    auto y = event.window.data2;

    for (auto &el : m_elements)
    {
      el->HandleResizeEvent({.x = 0,
                             .y = 0,
                             .w = static_cast<float>(x),
                             .h = static_cast<float>(y)});
    }
    return;
  }

  // handle mouse events
  if (event.type == SDL_EVENT_MOUSE_MOTION ||
      event.type == SDL_EVENT_WINDOW_RESIZED ||
      event.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
      event.type == SDL_EVENT_MOUSE_BUTTON_UP ||
      event.type == SDL_EVENT_MOUSE_WHEEL)
  {
    // if the focsed element has handled the event, just return
    if (auto el = m_focused.lock())
    {
      if (el->HandleMouseEvent(event))
        return;
    }

    for (auto &btn : m_elements)
    {
      // stop iterating, if event has been handled
      if (btn->HandleMouseEvent(event))
        return;
    }
  }
}

void UiManager::draw(SDL_Renderer *renderer)
{
  for (auto &btn : m_elements)
  {
    btn->draw(renderer);
  }

  // draw the focused element on top
  if (auto el = m_focused.lock())
    el->draw(renderer);
}

bool UiManager::isEventRelevant(SDL_Event &event)
{
  switch (event.type)
  {
  case SDL_EVENT_MOUSE_WHEEL:
  case SDL_EVENT_MOUSE_MOTION:
  case SDL_EVENT_WINDOW_RESIZED:
  case SDL_EVENT_MOUSE_BUTTON_DOWN:
  case SDL_EVENT_MOUSE_BUTTON_UP:
    return true;
  default:
    return false;
  }
}
