#ifndef AUDIO_HPP
#define AUDIO_HPP
#include <cstddef>
#include <string>

namespace lavrentev
{
  struct AudioChannel
  {
    float* samples = nullptr;
    size_t samplesSize = 0;
  };

  struct Audio
  {
    AudioChannel* channels = nullptr;
    size_t channelsSize = 0;
    float sampleRate = 0.0f;
    size_t bitDepth = 16;
    std::string name;
    std::string format;
  };

  void freeAudioMemory(Audio &audio);
  float getDuration(const Audio &audio);
}

#endif
