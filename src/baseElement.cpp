#include <sui/baseElement.hpp>

#include <sui/UiManager.hpp>
#include <sui/logger.hpp>

using namespace sui;

UiManager *BaseElement::s_uimanager = nullptr;

void BaseElement::focus()
{
  sui::Logger::Debug("Focusing element {}", (void *)this);
  s_uimanager->m_focused = this->shared_from_this();
}
