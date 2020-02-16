
#pragma once
#include "SDL.h"
#include "globals.h"

#include <math.h>
#include "sdl_dblclick.h"
#include "Colors.h"
#include "platform.h"
#include "Main_Window.h"
#include "Experience.h"
#include "Menu_Bar.h"
#include "gui/Window.h"

#include "Instruments.h"

typedef Uint32 WindowID;

struct Tracker
{
public:
  Tracker(int &argc, char **argv);
  ~Tracker();
  void run();
  void handle_events();

  Menu_Bar menu_bar;
  Main_Window main_window;

  SDL_DisplayMode monitor_display_mode;

  Experience *sub_window_experience = NULL;
  
  Options_Window options_window;
  Spc_Export_Window spc_export_window;
  static const int NUM_WINDOWS = 2;
  Window *window_map[NUM_WINDOWS+1];

  void update_fps(int fps);
  int fps;
  Uint32 frame_tick_timeout, frame_tick_duration;

  // TRACKER CORE
  Instrument instruments[NUM_INSTR];
};