#ifndef FAKE_SDL
#define FAKE_SDL
#include <cstddef>
#include <cstdint>
#define SDL_INIT_AUDIO 0
#define AUDIO_U8 8
#define AUDIO_S16LSB 16
#define AUDIO_S32LSB 32
using Uint16 = uint16_t;
using Uint32 = uint32_t;
using Uint8 = uint8_t;
using SDL_AudioDeviceID = Uint8;
struct SDL_AudioSpec
{
  Uint32 freq;
  Uint16 channels;
  Uint8 format;
  Uint16 samples;
  void *callback;
};
int SDL_Init(int);
int SDL_GetError();
void SDL_Delay(Uint32);
void SDL_Quit();
SDL_AudioDeviceID SDL_OpenAudioDevice(void *, int, SDL_AudioSpec *, void *, int);
void SDL_CloseAudioDevice(SDL_AudioDeviceID);
void SDL_PauseAudioDevice(SDL_AudioDeviceID, int);
void SDL_QueueAudio(SDL_AudioDeviceID, Uint8 *, size_t);
#endif
