#include <new>
#define MINIAUDIO_IMPLEMENTATION

#include "miniaudio.h"
#include <kfr/kfr.h>
#include <kfr/dsp.hpp>
#include "commands.hpp"

void lavrentev::viewFormats(std::istream&, std::ostream& os)
{
  os << "1: WAV" << '\n';
  os << "2: MP3" << '\n';
  os << "3: AAC" << '\n';
  os << "4: AIFF" << '\n';
  os << "5: FLAC" << '\n';
}

void lavrentev::help(std::istream&, std::ostream& os)
{
  os << "1: import - импортирует аудиофайл внутрь программы" << '\n';
  os << "2: view_all - выводит все файлы, находящиеся в оперативной памяти программы c их id" << '\n';
  os << "3: convert - конвертирует аудиофайл в другой формат, указанный пользователем" << '\n';
  os << "4: convert_to_wav - конвертирует аудиофайл в формат WAV" << '\n';
  os << "5: convert_to_mp3 - конвертирует аудиофайл в формат MP3" << '\n';
  os << "6: pitch - питчинг файла на <semitones> полутонов вверх/вниз" << '\n';
  os << "7: cut - обрезка файла c момента <start> длительностью <duration>" << '\n';
  os << "8: change_bitrate - меняет уровень сжатия файла на <new_bitrate>" << '\n';
  os << "9: normalize - нормализация аудиофайла" << '\n';
  os << "10: change_volume - изменяет громкость аудиофайла на <coefficient>" << '\n';
  os << "11: export - экспортирует аудиофайл c именем <name> в указанную папку <dir>" << '\n';
  os << "12: info - выводит техническую информацию o файле <file>" << '\n';
  os << "13: view_formats - выводит форматы, доступные для конвертации" << '\n';
  os << "14: help - выводит справку по доступным командам" << '\n';
}

void lavrentev::importFile(std::istream& is, std::ostream&, CCHT &data)
{
  std::string filePath;
  size_t id;
  is >> filePath >> id;

  ma_decoder decoder;
  ma_result result = ma_decoder_init_file(filePath.c_str(), nullptr, &decoder);
  if (result != MA_SUCCESS)
  {
    throw std::runtime_error("Failed to initialize decoder for file: " + filePath);
  }

  Audio newFile;
  newFile.sampleRate = static_cast<float>(decoder.outputSampleRate);
  newFile.channelsSize = decoder.outputChannels;
  newFile.bitDepth = 16; 

  ma_uint64 totalFrames = 0;
  ma_decoder_get_length_in_pcm_frames(&decoder, &totalFrames);

  size_t bufSize = totalFrames * newFile.channelsSize;
  float* temp = new float[bufSize];
  
  ma_uint64 framesRead = 0;
  ma_decoder_read_pcm_frames(&decoder, temp, totalFrames, &framesRead);
  ma_decoder_uninit(&decoder);

  try
  {
    newFile.channels = new AudioChannel[newFile.channelsSize];

    for (size_t i = 0; i < newFile.channelsSize; ++i) {
      newFile.channels[i].samples = new float[framesRead];
      newFile.channels[i].samplesSize = framesRead;
    }
  }
  catch (...)
  {
    freeAudioMemory(newFile); 
    throw std::bad_alloc();
  }
  for (ma_uint64 frame = 0; frame < framesRead; ++frame) {
    for (size_t chan = 0; chan < newFile.channelsSize; ++chan) {
      float sample = temp[frame * newFile.channelsSize + chan];
      newFile.channels[chan].samples[frame] = sample;
    }
  }
  delete[] temp;
  data[id] = newFile;
}

void lavrentev::viewAll(std::istream&, std::ostream& os, CCHT &data)
{
  data.print(os);
}
