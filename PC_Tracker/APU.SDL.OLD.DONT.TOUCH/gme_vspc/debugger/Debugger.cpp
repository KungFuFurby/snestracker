#include "Debugger.h"
#include <getopt.h>
#include "utility.h"
#include "Screen.h"


#define L_FLAG 0
#define R_FLAG 1

Debugger::Debugger(int &argc, char **argv) : //, Music_Player *player,
  //SDL_Window *win, SDL_Renderer *renderer, SDL_Texture *text, SDL_Surface *screen) : 
//player(player), sdlWindow(win), sdlRenderer(renderer),sdlTexture(text),screen(screen),
//main_memory_area(screen,player),
//mouseover_hexdump_area(player,screen),
main_window(argc,argv)
{
  BaseD::main_window = &main_window;
  BaseD::dsp_window = &dsp_window;
  BaseD::instr_window = &instr_window;
  exp = &main_window;
}


void Debugger::run()
{
  //SDL_CaptureMouse(SDL_TRUE);
  // gotta call this once to initialize important stuffz
  main_window.reload();
  main_window.one_time_draw();

  // exp is changed from BaseD
  while (!quitting)
  {
    exp->run();
    exp->draw();
    handle_events();
    //sdlfont_drawString(screen, 10,10, "back", Colors::red);

    //SDL_Delay( 1000 / 100 );
  }
}

void Debugger::handle_events()
{
  SDL_Event ev;
  while (SDL_PollEvent(&ev))
  {
    exp->receive_event(ev);
  }
}




