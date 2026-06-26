#ifndef DECODER_HPP
#define DECODER_HPP

#include <Vector.hpp>
#include <string>
#include "sdl_header.hpp"

namespace dirko
{

  struct WavFile
  {
    Uint16 audioFormat;
    Uint16 numChannels;
    Uint32 sampleRate;
    Uint32 byteRate;
    Uint16 blockAlign;
    Uint16 bitsPerSample;

    Vector< Uint8 > rawData;
  };

  void Load(const std::string &filename, WavFile &outWav);
}
#endif
