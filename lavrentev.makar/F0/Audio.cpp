#include "Audio.hpp"

void lavrentev::freeAudioMemory(Audio& audio)
{
  if (audio.channels != nullptr)
  {
    for (size_t i = 0; i < audio.channelsSize; ++i)
    {
      if (audio.channels[i].samples != nullptr)
      {
        delete[] audio.channels[i].samples;
        audio.channels[i].samples = nullptr;
      }
    }
    delete[] audio.channels;
    audio.channels = nullptr;
  }
  audio.channelsSize = 0;
}