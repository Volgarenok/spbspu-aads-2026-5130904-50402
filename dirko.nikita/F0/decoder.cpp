#include "decoder.hpp"
#include <cstring>
#include <fstream>
#include <stdexcept>

void dirko::Load(const std::string &filename, WavFile &outWav)
{
  std::ifstream file(filename, std::ios::binary);

  if (!file.is_open()) {
    throw std::invalid_argument("Failed to open file");
  }

  char riff[4];
  file.read(riff, 4);

  if (std::strncmp(riff, "RIFF", 4) != 0) {
    throw std::runtime_error("Not a RIFF file");
  }

  Uint32 chunkSize = 0;
  file.read(reinterpret_cast< char * >(&chunkSize), 4);

  char wave[4];
  file.read(wave, 4);

  if (std::strncmp(wave, "WAVE", 4) != 0) {
    throw std::runtime_error("Not a WAVE file");
  }

  bool fmtFound = false;
  bool dataFound = false;
  char chunkId[4];

  while (file.read(chunkId, 4)) {
    Uint32 subChunkSize = 0;
    file.read(reinterpret_cast< char * >(&subChunkSize), 4);

    if (std::strncmp(chunkId, "fmt ", 4) == 0) {
      fmtFound = true;

      file.read(reinterpret_cast< char * >(&outWav.audioFormat), 2);
      file.read(reinterpret_cast< char * >(&outWav.numChannels), 2);
      file.read(reinterpret_cast< char * >(&outWav.sampleRate), 4);
      file.read(reinterpret_cast< char * >(&outWav.byteRate), 4);
      file.read(reinterpret_cast< char * >(&outWav.blockAlign), 2);
      file.read(reinterpret_cast< char * >(&outWav.bitsPerSample), 2);

      if (subChunkSize > 16) {
        file.seekg(subChunkSize - 16, std::ios::cur);
      }
    } else if (std::strncmp(chunkId, "data", 4) == 0) {
      dataFound = true;

      outWav.rawData.resize(subChunkSize);
      file.read(reinterpret_cast< char * >(outWav.rawData.getData()), subChunkSize);
    } else {
      file.seekg(subChunkSize, std::ios::cur);
    }
  }

  if (!fmtFound || !dataFound) {
    throw std::runtime_error("Invalid WAV file");
  }

  if (outWav.audioFormat != 1) {
    throw std::runtime_error("Only PCM supported");
  }
}
