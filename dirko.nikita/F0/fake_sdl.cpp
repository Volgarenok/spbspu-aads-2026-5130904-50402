#include "fake_sdl.hpp"

#define USE_FAKE_SDL
#ifdef USE_FAKE_SDL
int SDL_Init(int)
{
  return 0;
}
int SDL_GetError()
{
  return 0;
}
void SDL_Delay(Uint32)
{}
void SDL_Quit()
{}
SDL_AudioDeviceID SDL_OpenAudioDevice(void *, int, SDL_AudioSpec *, void *, int)
{
  return 1;
}
void SDL_CloseAudioDevice(SDL_AudioDeviceID)
{}
void SDL_PauseAudioDevice(SDL_AudioDeviceID, int)
{}
void SDL_QueueAudio(SDL_AudioDeviceID, Uint8 *, size_t)
{}
#endif
