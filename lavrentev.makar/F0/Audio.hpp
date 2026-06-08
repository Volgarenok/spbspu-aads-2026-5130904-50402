#ifndef AUDIO_HPP
#define AUDIO_HPP
#include <vector>

namespace lavrentev
{
  using AudioChnl = std::vector<float>;
  using AudioBuf = std::vector<AudioChnl>;

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
  };

  void freeAudioMemory(Audio& audio);
}

#endif