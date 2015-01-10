#include "BaseD.h"
/*Music_Player *BaseD::player;
SDL_Surface *BaseD::screen;
uint8_t *BaseD::IAPURAM;*/
#include "Main_Window.h"

int BaseD::grand_mode=GrandMode::MAIN;
//int BaseD::submode=0;
BaseD::Cfg BaseD::g_cfg;// = { 0,0,0,0,0,0,DEFAULT_SONGTIME,0,0,0,0,0,NULL };

int BaseD::g_paused = 0;
uint8_t * BaseD::IAPURAM=NULL;
bool BaseD::quitting=false;
unsigned char BaseD::packed_mask[32];

int BaseD::g_cur_entry= 0;
bool BaseD::paused;

int BaseD::song_time;
track_info_t BaseD::tag;
char * BaseD::g_real_filename=NULL;
char BaseD::now_playing[1024];
//bool BaseD::is_onetime_draw_necessary=false;


Experience * BaseD::exp=NULL;
Main_Window * BaseD::main_window=NULL;
Dsp_Window * BaseD::dsp_window=NULL;

const char * BaseD::path=NULL;
Voice_Control BaseD::voice_control;

void BaseD::update_track_tag()
{
  //update_window_title();
  tag = player->track_info();


  /* decide how much time the song will play */
  if (!g_cfg.ignoretagtime) {
    song_time = (int)tag.length / 1000; //atoi((const char *)tag.seconds_til_fadeout);
    if (song_time <= 0) {
      song_time = g_cfg.defaultsongtime;
    }
  }
  else {
    song_time = g_cfg.defaultsongtime;
  }

  song_time += g_cfg.extratime;

  now_playing[0] = 0;
  if (tag.song)
  {
    if (strlen((const char *)tag.song)) {
      sprintf(now_playing, "Now playing: %s (%s), dumped by %s\n", tag.song, tag.game, tag.dumper);
    }   
  }
  if (strlen(now_playing)==0) {
    sprintf(now_playing, "Now playing: %s\n", g_cfg.playlist[g_cur_entry]);
  }
}

void BaseD::start_track( int track, const char* path )
{
  //paused = false;
  //if (!player->is_paused())
  handle_error( player->start_track( track - 1 ) );
  // update window title with track info
  
  long seconds = player->track_info().length / 1000;
  const char* game = player->track_info().game;
  if ( !*game )
  {
    // extract filename
    game = strrchr( path, '\\' ); // DOS
    if ( !game )
      game = strrchr( path, '/' ); // UNIX
    if ( !game )
    {
      if (path)
        game = path;
      else game = "";
    }
    else
      game++; // skip path separator
  }
  
  char title [512];
  sprintf( title, "%s: %d/%d %s (%ld:%02ld)",
      game, track, player->track_count(), player->track_info().song,
      seconds / 60, seconds % 60 );
  SDL_SetWindowTitle(sdlWindow, title);
}

void BaseD::reload()
{
  if (g_cfg.playlist[g_cur_entry] == NULL)
    exit (2);

  #ifdef WIN32
  g_real_filename = strrchr(g_cfg.playlist[g_cur_entry], '\\');
#else
  g_real_filename = strrchr(g_cfg.playlist[g_cur_entry], '/');
#endif
  if (!g_real_filename) {
    g_real_filename = g_cfg.playlist[g_cur_entry];
  }
  else {
    // skip path sep
    g_real_filename++;
  } 
  //main_window->reload();
  // Load file
  path = g_cfg.playlist[g_cur_entry];
  handle_error( player->load_file( g_cfg.playlist[g_cur_entry] ) );
  
  IAPURAM = player->spc_emu()->ram();
  //Memory::IAPURAM = IAPURAM;
  
  // report::memsurface.init
  report::memsurface.clear();

  memset(report::used, 0, sizeof(report::used));
  memset(report::used2, 0, sizeof(report::used2));
  //if (!mouseover_hexdump_area.address)mouseover_hexdump_area.address =0;
  report::last_pc = -1;
  
  start_track( 1, path );
  voice_control.was_keyed_on = 0;
  player->mute_voices(voice_control.muted);
  player->ignore_silence();

  // update Window Title
  long seconds = player->track_info().length / 1000;
  const char* game = player->track_info().game;
  if ( !*game )
  {
    // extract filename
    game = strrchr( path, '\\' ); // DOS
    if ( !game )
      game = strrchr( path, '/' ); // UNIX
    if ( !game )
      game = path;
    else
      game++; // skip path separator
  }

  //main_window->is_onetime_draw_necessary=true;
  update_track_tag();
  if (grand_mode == MAIN)
    main_window->draw_track_tag();

  char title [512];
  sprintf( title, "%s: %d/%d %s (%ld:%02ld)",
      game, g_cur_entry+1, g_cfg.num_files, player->track_info().song,
      seconds / 60, seconds % 60 );
  SDL_SetWindowTitle(sdlWindow, title);

}

void BaseD::toggle_pause()
{
  player->toggle_pause();
}

void BaseD::restart_track()
{
  SDL_PauseAudio(1);
  g_cur_entry=0;
  player->pause(0);
  reload();
}

void BaseD::prev_track()
{
  SDL_PauseAudio(true);
  g_cur_entry--;
  if (g_cur_entry<0) { g_cur_entry = g_cfg.num_files-1; }
  reload();
}

void BaseD::next_track()
{
  SDL_PauseAudio(true);
  g_cur_entry++;
  if (g_cur_entry>=g_cfg.num_files) { g_cur_entry = 0; }
  reload();
}

void BaseD::draw_menu_bar()
{
  sprintf(tmpbuf, " QUIT - PAUSE - RESTART - PREV - NEXT - WRITE MASK - DSP MAP");
  sdlfont_drawString(screen, 0, screen->h-9, tmpbuf, Colors::yellow);
}

void BaseD::restart_current_track()
{
  report::memsurface.clear();
  player->start_track(0); // based on only having 1 track
  player->pause(0);
  // in the program .. this would have to change otherwise
}

void BaseD::switch_mode(int mode)
{
  grand_mode = mode;
  clear_screen();
  draw_menu_bar();
  if (mode == GrandMode::MAIN)
  {
    if (main_window)
      exp = (Experience*)main_window;
    else 
    {
      fprintf(stderr, "NO MAIN WINDOW!?!\n");
      exit(2);
    }
    main_window->one_time_draw();
  }
  else if (mode == GrandMode::DSP_MAP)
  {
    if (dsp_window)
      exp = (Experience*)dsp_window;
    else 
    {
      fprintf(stderr, "NO DSPWINDOW!?!\n");
      exit(2);
    }
  }
}


void BaseD::pack_mask(unsigned char packed_mask[32])
{
  int i;
  
  memset(packed_mask, 0, 32);
  for (i=0; i<256; i++)
  {
    if (report::used2[i])
    packed_mask[i/8] |= 128 >> (i%8);
  }
}

void BaseD::applyBlockMask(char *filename)
{
  FILE *fptr;
  unsigned char nul_arr[256];
  int i;

  memset(nul_arr, g_cfg.filler, 256);
  
  fptr = fopen(filename, "r+");
  if (!fptr) { perror("fopen"); }

  printf("[");
  for (i=0; i<256; i++)
  {
    fseek(fptr, 0x100+(i*256), SEEK_SET);
    
    if (!report::used2[i]) {
      printf(".");
      fwrite(nul_arr, 256, 1, fptr);
    } else {
      printf("o");
    }
    fflush(stdout);
  }
  printf("]\n");
  
  fclose(fptr);
}


void BaseD::write_mask(unsigned char packed_mask[32])
{
  FILE *msk_file;
  char *sep;
  char filename[1024];
  unsigned char tmp;
  int i;
  strncpy(filename, g_cfg.playlist[g_cur_entry], 1024);
#ifdef WIN32
  sep = strrchr(filename, '\\');
#else
  sep = strrchr(filename, '/');
#endif
  // keep only the path
  if (sep) { sep++; *sep = 0; } 
  else { 
    filename[0] = 0; 
  }

  // add the filename
  strncat(filename, g_real_filename, 1024);

  // but remove the extension if any
  sep = strrchr(filename, '.');
  if (sep) { *sep = 0; }

  // and use the .msk extension
  strncat(filename, ".msk", 1024);

  msk_file = fopen(filename, "wb");
  if (!msk_file) {
    perror("fopen");
  }
  else
  {
    fwrite(packed_mask, 32, 1, msk_file);
  }

  printf("Writing mask to '%s'\n", filename);

  // the first 32 bytes are for the 256BytesBlock mask
  printf("256 Bytes-wide block mask:\n");
  for (i=0; i<32; i++) {
    printf("%02X",packed_mask[i]);
  }
  printf("\n");

  printf("Byte level mask..."); fflush(stdout);
  memset(packed_mask, 0, 32);
  for (i=0; i<65536; i+=8)
  {
    tmp = 0;
    if (report::used[i]) tmp |= 128;
    if (report::used[i+1]) tmp |= 64;
    if (report::used[i+2]) tmp |= 32;
    if (report::used[i+3]) tmp |= 16;
    if (report::used[i+4]) tmp |= 8;
    if (report::used[i+5]) tmp |= 4;
    if (report::used[i+6]) tmp |= 2;
    if (report::used[i+7]) tmp |= 1;
    fwrite(&tmp, 1, 1, msk_file);
  }
  printf("Done.\n");
  fclose(msk_file);
}