// SPDX-FileCopyrightText: 2019-2024 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: (GPL-3.0 OR CC-BY-NC-ND-4.0)

#pragma once
#include "input_source.h"

#include <SDL.h>

//class SDLKeyboardMouse final : public InputSource
class SDLKeyboardMouse
{
public:
  SDLKeyboardMouse();
  ~SDLKeyboardMouse();
  void ProcessSDLKeyboardMouseEvent(const SDL_Event* ev);
  bool Initialize(SettingsInterface& si, std::unique_lock<std::mutex>& settings_lock);
  
private:
  bool InitializeSubsystem();
  void ShutdownSubsystem();
  void ProcessPlatformKeyEvent(s32 key, bool pressed);
  void ProcessPlatformMouseMoveEvent(float x, float y);
  void ProcessPlatformMouseButtonEvent(s32 button, bool pressed);
  void ProcessPlatformMouseWheelEvent(float x, float y);
  bool m_sdl_subsystem_initialized = false;
};
