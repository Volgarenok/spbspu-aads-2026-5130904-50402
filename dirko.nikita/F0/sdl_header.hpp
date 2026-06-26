#ifndef SDL_HEADER_HPP
#define SDL_HEADER_HPP

#define USE_FAKE_SDL
#ifdef USE_FAKE_SDL
#include "fake_sdl.hpp"
#else
#include <SDL2/SDL.h>
#endif
#endif
