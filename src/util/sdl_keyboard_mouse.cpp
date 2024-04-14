#include "sdl_keyboard_mouse.h"

#include "core/host.h"

#include "common/log.h"

Log_SetChannel(SDLKeyboardMouse);

SDLKeyboardMouse::SDLKeyboardMouse()
{
  InitializeSubsystem();
}

SDLKeyboardMouse::~SDLKeyboardMouse()
{
  ShutdownSubsystem();
}

bool SDLKeyboardMouse::InitializeSubsystem()
{
  if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
  {
    Log_ErrorPrint("SDL_InitSubSystem(SDL_INIT_EVENTS) failed");
    return false;
  }
  m_sdl_subsystem_initialized = true;
  return true;
}

void SDLKeyboardMouse::ShutdownSubsystem()
{
  if (m_sdl_subsystem_initialized)
  {
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    m_sdl_subsystem_initialized = false;
  }
}

void SDLKeyboardMouse::ProcessPlatformKeyEvent(s32 key, bool pressed)
{
  Host::RunOnCPUThread([key, pressed]() {
    InputManager::InvokeEvents(InputManager::MakeHostKeyboardKey(key), pressed ? 1.0f : 0.0f,
                               GenericInputBinding::Unknown);
  });
}

void SDLKeyboardMouse::ProcessPlatformMouseMoveEvent(float x, float y)
{
  InputManager::UpdatePointerAbsolutePosition(0, x, y);
  //ImGuiManager::UpdateMousePosition(x, y);
}

void SDLKeyboardMouse::ProcessPlatformMouseButtonEvent(s32 button, bool pressed)
{
  Host::RunOnCPUThread([button, pressed]() {
    InputManager::InvokeEvents(InputManager::MakePointerButtonKey(0, button), pressed ? 1.0f : 0.0f,
                               GenericInputBinding::Unknown);
  });
}

void SDLKeyboardMouse::ProcessPlatformMouseWheelEvent(float x, float y)
{
  if (x != 0.0f)
    InputManager::UpdatePointerRelativeDelta(0, InputPointerAxis::WheelX, x);
  if (y != 0.0f)
    InputManager::UpdatePointerRelativeDelta(0, InputPointerAxis::WheelY, y);
}

void SDLKeyboardMouse::ProcessSDLKeyboardMouseEvent(const SDL_Event* ev)
{
  switch (ev->type)
  {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
    {
      const bool pressed = (ev->type == SDL_KEYDOWN);
      ProcessPlatformKeyEvent(static_cast<s32>(ev->key.keysym.sym), pressed);
    }
    break;

    case SDL_MOUSEMOTION:
    {
      const float x = static_cast<float>(ev->motion.x);
      const float y = static_cast<float>(ev->motion.y);
      ProcessPlatformMouseMoveEvent(x, y);
    }
    break;

    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    {
      const bool pressed = (ev->type == SDL_MOUSEBUTTONDOWN);
      if (ev->button.button > 0)
        ProcessPlatformMouseButtonEvent(ev->button.button - 1, pressed);
    }
    break;

    case SDL_MOUSEWHEEL:
    {
      ProcessPlatformMouseWheelEvent(ev->wheel.preciseX, ev->wheel.preciseY);
    }
    break;

    default:
    break;
  }
}
