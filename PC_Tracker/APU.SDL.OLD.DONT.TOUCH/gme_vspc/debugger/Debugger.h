
#pragma once
#include "SDL.h"
#include "globals.h"

#include "report.h"
#include "Voice_Control.h"
#include <math.h>
#include "sdl_dblclick.h"
#include "Port_Tool.h"
#include "Mouse_Hexdump_Area.h"
#include "Main_Memory_Area.h"
#include "Colors.h"
#include "platform.h"
#include "Main_Window.h"
#include "Dsp_Window.h"
#include "Experience.h"

struct Debugger : BaseD
{
public:
  Debugger(int &argc, char **argv); // , Music_Player *player, SDL_Window *, SDL_Renderer *, SDL_Texture *, SDL_Surface*);
  void run();
  void handle_events();

  Main_Window main_window;
  Dsp_Window dsp_window;
};
