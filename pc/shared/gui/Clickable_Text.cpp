#include "gui/Clickable_Text.h"
#include "utility.h"
#include "sdlfont.h"
#include <assert.h>

Clickable_Text::Clickable_Text(const char *str/*=""*/,
  int (*action)(void *data)/*=NULL*/,
  void *data/*=NULL*/,
  Uint32 color/*=Colors::Interface::color[Colors::Interface::text_fg]*/) :
    Clickable_Rect(action,data),
    str(str),
    color(color)
{
  init_width_height();
}
Clickable_Text::Clickable_Text(const char *str, int x, int y,
int (*action)(void *data)/*=NULL*/, void *data/*=NULL*/) :
Clickable_Rect(action,data),
str(str)
{
  setup(x,y);
}

void Clickable_Text::init_width_height()
{
  rect.w = strlen(str)*CHAR_WIDTH; // could add padding
  rect.h = CHAR_HEIGHT;
}

void Clickable_Text::setup(int x, int y, bool center_x/*=false*/)
{
  rect.w = strlen(str)*CHAR_WIDTH; // could add padding
  rect.h = CHAR_HEIGHT;
  rect.x = x;
  rect.y = y;

  if (center_x)
  {
    rect.x = (x / 2) - ((strlen(str) / 2) * CHAR_WIDTH);
  }
}

void Clickable_Text::draw(Uint32 color, bool prefill/*=true*/, 
  bool Vflip/*=false*/, bool Hflip/*=false*/, SDL_Surface *screen/*=RenderContext::screen*/)
{
  sdlfont_drawString(screen, rect.x, rect.y, str, color, 
    enabled ? Colors::Interface::color[Colors::Interface::Type::text_bg] : Colors::nearblack,
    prefill, Vflip, Hflip);
}

void Clickable_Text::draw(bool prefill/*=true*/, 
  bool Vflip/*=false*/, bool Hflip/*=false*/, SDL_Surface *screen/*=RenderContext::screen*/)
{
  draw(color, prefill, Vflip, Hflip, screen);
}
