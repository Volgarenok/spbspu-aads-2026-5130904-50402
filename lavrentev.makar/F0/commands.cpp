#include <new>
#include <stdexcept>
#include <cmath>
#define MINIAUDIO_IMPLEMENTATION

#include "miniaudio.h"
#include <kfr/kfr.h>
#include <kfr/dsp.hpp>
#include "commands.hpp"
#include "Audio.hpp"

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
  newFile.name = extractName(filePath);
  newFile.format = extractFormat(filePath);

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

std::string extractName(const std::string& filePath) {
  size_t last = filePath.find_last_of("/\\");

  if (last == std::string::npos)
  {
    return filePath;
  }

  return filePath.substr(last + 1);
}

std::string extractFormat(const std::string& filePath) {
  size_t last= filePath.find_last_of(".");

  if (last == std::string::npos || last == filePath.length() - 1)
  {
    throw std::invalid_argument("Invalid data");
  }

  return filePath.substr(last + 1);
}

void lavrentev::info(std::istream &is, std::ostream &os, CCHT &cht)
{
  size_t id;
  is >> id;
  if (!cht.has(id))
  {
    throw std::out_of_range("No such file");
  }
  Audio curr = cht[id];
  os << "Name: " << curr.name << '\n';
  os << "Format: " << curr.format << '\n';
  os << "Duration: " << getDuration(curr) << '\n';
  os << "Channels: " << curr.channelsSize << '\n';
  os << "Sample rate: " << curr.sampleRate << '\n';
  os << "Bit depth: " << curr.bitDepth << '\n';
}

void lavrentev::exportFile(std::istream &is, std::ostream&, CCHT &cht)
{
  size_t id;
  std::string fileName, path;
  is >> id >> fileName >> path;
  if (!cht.has(id))
  {
    throw std::out_of_range("No such file");
  }
  path += (fileName + "." + cht[id].format);

  size_t frames = cht[id].channels[0].samplesSize;
  size_t totalChannels = cht[id].channelsSize;
  ma_encoder_config config = ma_encoder_config_init(
    ma_encoding_format_wav, 
    ma_format_f32, 
    static_cast<ma_uint32>(totalChannels), 
    static_cast<ma_uint32>(cht[id].sampleRate)
  );

  ma_encoder encoder;
  ma_result result = ma_encoder_init_file(path.c_str(), &config, &encoder);
  if (result != MA_SUCCESS) {
    throw std::runtime_error("Failed to initialize encoder for file: " + fileName);
  }

  float* temp = nullptr;
  try
  {
    temp = new float[frames * totalChannels];
  }
  catch (const std::bad_alloc&)
  {
    ma_encoder_uninit(&encoder);
    throw std::logic_error("Export error");
  }

  for (size_t frame = 0; frame < frames; ++frame) {
    for (size_t chan = 0; chan < totalChannels; ++chan) {
      temp[frame * totalChannels + chan] = cht[id].channels[chan].samples[frame];
    }
  }
  ma_uint64 framesWritten = 0;
  ma_encoder_write_pcm_frames(&encoder, temp, frames, &framesWritten);

  delete[] temp;
  ma_encoder_uninit(&encoder);
}

void lavrentev::changeVolume(std::istream &is, std::ostream&, CCHT &cht)
{
  size_t id;
  float coef;
  is >> id >> coef;

  if (!cht.has(id))
  {
    throw std::out_of_range("No such file");
  }
  if (coef > 5.0 || coef < 0)
  {
    throw std::out_of_range("Invalid coefficient");
  }

  Audio &curr = cht[id];
  for(size_t i = 0; i < curr.channelsSize; ++i)
  {
    for(size_t j = 0; j < curr.channels[i].samplesSize; ++j)
    {
      curr.channels[i].samples[j] *= coef;
      if (curr.channels[i].samples[j] > 1.0f)
      {
        curr.channels[i].samples[j] = 1.0f;
      }
      else if (curr.channels[i].samples[j] < -1.0f)
      {
        curr.channels[i].samples[j] = -1.0f;
      }
    }
  }
}

void lavrentev::normalize(std::istream &is, std::ostream&, CCHT &cht)
{
  size_t id;
  is >> id;
  if (!cht.has(id))
  {
    throw std::out_of_range("No such file");
  }

  Audio &curr = cht[id];
  float maxPeak = 0.0f;
  for(size_t i = 0; i < curr.channelsSize; ++i)
  {
    for(size_t j = 0; j < curr.channels[i].samplesSize; ++j)
    {
      if (abs(curr.channels[i].samples[j]) > maxPeak)
      {
        maxPeak = curr.channels[i].samples[j];
      }
    }
  }
  if (maxPeak == 0)
  {
    return;
  }
  double targetMax = 1.0f;
  float gain = targetMax / maxPeak;
  for(size_t i = 0; i < curr.channelsSize; ++i)
  {
    for(size_t j = 0; j < curr.channels[i].samplesSize; ++j)
    {
      curr.channels[i].samples[j] *= gain;
      if (curr.channels[i].samples[j] > 1.0f)
      {
        curr.channels[i].samples[j] = 1.0f;
      }
      else if (curr.channels[i].samples[j] < -1.0f)
      {
        curr.channels[i].samples[j] = -1.0f;
      }
    }
  }
}

void lavrentev::changeBitrate(std::istream &is, std::ostream&, CCHT &cht)
{
  size_t id, newBR;
  is >> id >> newBR;
  if (!cht.has(id))
  {
    throw std::out_of_range("No such file");
  }
  cht[id].bitDepth = newBR;
}

void lavrentev::cut(std::istream &is, std::ostream&, CCHT &cht)
{
  size_t id;
  float start, duration;
  is >> id >> start >> duration;
  if (!cht.has(id))
  {
    throw std::out_of_range("No such file");
  }

  Audio &curr = cht[id];
  float startBit = start * curr.sampleRate;
  float durBit = duration * curr.sampleRate;

  if (startBit + durBit >= curr.channels[0].samplesSize || startBit < 0.0f)
  {
    throw std::out_of_range("Invalid segment");
  }

  Audio newAudio;
  AudioChannel* newChannels = new AudioChannel[curr.channelsSize];
  newAudio.channels = newChannels;
  newAudio.channelsSize = curr.channelsSize;
  newAudio.bitDepth = curr.bitDepth;
  newAudio.format = curr.format;
  newAudio.name = curr.name;
  newAudio.sampleRate = curr.sampleRate;

  for (size_t i = 0; i < newAudio.channelsSize; ++i)
  {
    newChannels[i].samples = nullptr;
    newChannels[i].samplesSize = 0;
  }

  try
  {
    for(size_t i = 0; i < curr.channelsSize; ++i)
    {
      float* newSample = new float[durBit];
      size_t idx = 0;
      for(size_t j = static_cast<size_t>(startBit); j < static_cast<size_t>(startBit + durBit); ++j)
      {
        newSample[idx] = curr.channels[i].samples[j];
        ++idx;
      }
      newChannels[i].samples = newSample;
      newChannels[i].samplesSize = durBit;
    }
  }
  catch(...)
  {
    freeAudioMemory(newAudio);
    throw std::bad_alloc();
  }
  std::swap(curr, newAudio);
  freeAudioMemory(newAudio);
}
