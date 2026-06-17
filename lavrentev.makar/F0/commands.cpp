#include <new>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <sstream>
#include "Audio.hpp"

namespace lavrentev
{
  std::ostream &operator<<(std::ostream &os, const Audio &audio)
  {
    os << "Name: " << audio.name << "\n"
       << "Format: " << audio.format << "\n"
       << "Sample Rate: " << audio.sampleRate << "\n"
       << "Bit Depth: " << audio.bitDepth;
    return os;
  }
}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wconversion"
#endif

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.inc"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif
#include "commands.hpp"

namespace
{
  std::string trim(const std::string &text)
  {
    const std::string spaces = " \t\r\n";
    size_t first = text.find_first_not_of(spaces);
    if (first == std::string::npos)
    {
      return "";
    }
    size_t last = text.find_last_not_of(spaces);
    return text.substr(first, last - first + 1);
  }

  bool parseId(const std::string &text, size_t &id)
  {
    if (text.empty())
    {
      return false;
    }
    for (char c: text)
    {
      if (c < '0' || c > '9')
      {
        return false;
      }
    }
    std::istringstream input(text);
    input >> id;
    return !input.fail();
  }

  size_t findFreeId(const lavrentev::CCHT &data)
  {
    size_t id = 1;
    while (data.has(id))
    {
      ++id;
    }
    return id;
  }

  void parseImportArgs(const std::string &line, std::string &filePath, size_t &id, lavrentev::CCHT &data)
  {
    std::string args = trim(line);
    if (args.empty())
    {
      throw std::invalid_argument("No import arguments");
    }

    id = findFreeId(data);
    if (args[0] == '"' || args[0] == '\'')
    {
      char quote = args[0];
      size_t end = args.find(quote, 1);
      if (end == std::string::npos)
      {
        throw std::invalid_argument("Invalid file path");
      }
      filePath = args.substr(1, end - 1);
      std::string rest = trim(args.substr(end + 1));
      if (!rest.empty() && !parseId(rest, id))
      {
        throw std::invalid_argument("Invalid file id");
      }
    }
    else
    {
      filePath = args;
      size_t lastSpace = args.find_last_of(" \t");
      if (lastSpace != std::string::npos)
      {
        size_t parsedId = 0;
        std::string lastToken = trim(args.substr(lastSpace + 1));
        if (parseId(lastToken, parsedId))
        {
          id = parsedId;
          filePath = trim(args.substr(0, lastSpace));
        }
      }
    }

    filePath = trim(filePath);
    if (filePath.empty())
    {
      throw std::invalid_argument("Invalid file path");
    }
    if (data.has(id))
    {
      throw std::out_of_range("ID already exists");
    }
  }
}

void lavrentev::viewFormats(std::istream &, std::ostream &os, CCHT &)
{
  os << "1: WAV" << '\n';
  os << "2: MP3" << '\n';
  os << "3: AAC" << '\n';
  os << "4: AIFF" << '\n';
  os << "5: FLAC" << '\n';
}

void lavrentev::help(std::istream &, std::ostream &os, CCHT &)
{
  os << "1: import <path> [id] - импортирует аудиофайл внутрь программы" << '\n';
  os << "2: view_all - выводит все файлы, находящиеся в оперативной памяти "
    "программы c их id" << '\n';
  os << "3: convert <id> <new_id> <format> - конвертирует аудиофайл в другой формат, указанный "
    "пользователем" << '\n';
  os << "4: convert_to_wav <id> <new_id> - конвертирует аудиофайл в формат WAV" << '\n';
  os << "5: convert_to_mp3 <id> <new_id> - конвертирует аудиофайл в формат MP3" << '\n';
  os << "6: pitch <id> <new_id> <semitones> - питчинг файла на <semitones> полутонов вверх/вниз" << '\n';
  os << "7: cut <id> <start> <duration> - обрезка файла c момента <start> длительностью <duration>" << '\n';
  os << "8: change_bitrate <id> <new_bitrate> - меняет уровень сжатия файла на <new_bitrate>" << '\n';
  os << "9: normalize <id> - нормализация аудиофайла" << '\n';
  os << "10: change_volume <id> <coefficient> - изменяет громкость аудиофайла на <coefficient>" << '\n';
  os << "11: export <id> <name> <dir> - экспортирует аудиофайл c именем <name> в указанную папку "
    "<dir>" << '\n';
  os << "12: info <id> - выводит техническую информацию o файле" << '\n';
  os << "13: view_formats - выводит форматы, доступные для конвертации" << '\n';
  os << "14: help - выводит справку по доступным командам" << '\n';
}

void lavrentev::importFile(std::istream &is, std::ostream &os, CCHT &data)
{
  std::string args;
  std::string filePath;
  size_t id = 0;
  std::getline(is >> std::ws, args);
  parseImportArgs(args, filePath, id, data);

  ma_decoder decoder;
  ma_result result = ma_decoder_init_file(
    filePath.c_str(),
    nullptr,
    &decoder
  );
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
  float *temp = new float[bufSize];

  ma_uint64 framesRead = 0;
  ma_decoder_read_pcm_frames(
    &decoder,
    temp,
    totalFrames,
    &framesRead
  );
  ma_decoder_uninit(&decoder);

  try
  {
    newFile.channels = new AudioChannel[newFile.channelsSize];

    for (size_t i = 0; i < newFile.channelsSize; ++i)
    {
      newFile.channels[i].samples = new float[framesRead];
      newFile.channels[i].samplesSize = framesRead;
    }
  } catch (...)
  {
    freeAudioMemory(newFile);
    throw std::bad_alloc();
  }
  for (ma_uint64 frame = 0; frame < framesRead; ++frame)
  {
    for (size_t chan = 0; chan < newFile.channelsSize; ++chan)
    {
      float sample = temp[frame * newFile.channelsSize + chan];
      newFile.channels[chan].samples[frame] = sample;
    }
  }
  delete[] temp;
  data[id] = newFile;
  os << "Imported: " << id << '\n';
}

void lavrentev::viewAll(std::istream &, std::ostream &os, CCHT &data)
{
  data.print(os);
}

std::string lavrentev::extractName(const std::string &filePath)
{
  size_t last = filePath.find_last_of("/\\");

  if (last == std::string::npos)
  {
    return filePath;
  }

  return filePath.substr(last + 1);
}

std::string lavrentev::extractFormat(const std::string &filePath)
{
  size_t last = filePath.find_last_of(".");

  if (last == std::string::npos || last == filePath.length() - 1)
  {
    throw std::invalid_argument("Invalid data");
  }

  return filePath.substr(last + 1);
}

void lavrentev::info(std::istream &is, std::ostream &os, CCHT &cht)
{
  size_t id;
  if (!(is >> id))
  {
    throw std::invalid_argument("Invalid arguments");
  }
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

void lavrentev::exportFile(std::istream &is, std::ostream &, CCHT &cht)
{
  size_t id;
  std::string fileName, path;
  if (!(is >> id >> fileName >> path))
  {
    throw std::invalid_argument("Invalid arguments");
  }
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
  if (result != MA_SUCCESS)
  {
    throw std::runtime_error("Failed to initialize encoder for file: " + fileName);
  }

  float *temp = nullptr;
  try
  {
    temp = new float[frames * totalChannels];
  }
  catch (const std::bad_alloc &)
  {
    ma_encoder_uninit(&encoder);
    throw std::logic_error("Export error");
  }

  for (size_t frame = 0; frame < frames; ++frame)
  {
    for (size_t chan = 0; chan < totalChannels; ++chan)
    {
      temp[frame * totalChannels + chan] = cht[id].channels[chan].samples[frame];
    }
  }
  ma_uint64 framesWritten = 0;
  ma_encoder_write_pcm_frames(
    &encoder,
    temp,
    frames,
    &framesWritten
  );

  delete[] temp;
  ma_encoder_uninit(&encoder);
}

void lavrentev::changeVolume(std::istream &is, std::ostream &, CCHT &cht)
{
  size_t id;
  float coef;
  if (!(is >> id >> coef))
  {
    throw std::invalid_argument("Invalid arguments");
  }

  if (!cht.has(id))
  {
    throw std::out_of_range("No such file");
  }
  if (coef > 5.0 || coef < 0)
  {
    throw std::out_of_range("Invalid coefficient");
  }

  Audio &curr = cht[id];
  for (size_t i = 0; i < curr.channelsSize; ++i)
  {
    for (size_t j = 0; j < curr.channels[i].samplesSize; ++j)
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

void lavrentev::normalize(std::istream &is, std::ostream &, CCHT &cht)
{
  size_t id;
  if (!(is >> id))
  {
    throw std::invalid_argument("Invalid arguments");
  }
  if (!cht.has(id))
  {
    throw std::out_of_range("No such file");
  }

  Audio &curr = cht[id];
  float maxPeak = 0.0f;
  for (size_t i = 0; i < curr.channelsSize; ++i)
  {
    for (size_t j = 0; j < curr.channels[i].samplesSize; ++j)
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
  for (size_t i = 0; i < curr.channelsSize; ++i)
  {
    for (size_t j = 0; j < curr.channels[i].samplesSize; ++j)
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

void lavrentev::changeBitrate(std::istream &is, std::ostream &, CCHT &cht)
{
  size_t id, newBR;
  if (!(is >> id >> newBR))
  {
    throw std::invalid_argument("Invalid arguments");
  }
  if (!cht.has(id))
  {
    throw std::out_of_range("No such file");
  }
  cht[id].bitDepth = newBR;
}

void lavrentev::cut(std::istream &is, std::ostream &, CCHT &cht)
{
  size_t id;
  float start, duration;
  if (!(is >> id >> start >> duration))
  {
    throw std::invalid_argument("Invalid arguments");
  }
  if (!cht.has(id))
  {
    throw std::out_of_range("No such file");
  }

  Audio &curr = cht[id];
  size_t startBit = static_cast<size_t>(start * curr.sampleRate);
  size_t durBit = static_cast<size_t>(duration * curr.sampleRate);

  if (start < 0.0f || duration <= 0.0f || startBit + durBit >= curr.channels[0].samplesSize)
  {
    throw std::out_of_range("Invalid segment");
  }

  Audio newAudio;
  AudioChannel *newChannels = new AudioChannel[curr.channelsSize];
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
    for (size_t i = 0; i < curr.channelsSize; ++i)
    {
      float *newSample = new float[durBit];
      size_t idx = 0;
      for (size_t j = static_cast<size_t>(startBit); j < static_cast<size_t>(startBit + durBit); ++j)
      {
        newSample[idx] = curr.channels[i].samples[j];
        ++idx;
      }
      newChannels[i].samples = newSample;
      newChannels[i].samplesSize = durBit;
    }
  }
  catch (...)
  {
    freeAudioMemory(newAudio);
    throw std::bad_alloc();
  }
  std::swap(curr, newAudio);
  freeAudioMemory(newAudio);
}

void lavrentev::convert(std::istream &is, std::ostream &, CCHT &cht)
{
  size_t id, newId;
  std::string format;
  if (!(is >> id >> newId >> format))
  {
    throw std::invalid_argument("Invalid arguments");
  }
  bool c1 = format != "mp3" && format != "MP3";
  bool c2 = format != "wav" && format != "WAV";
  bool c3 = format != "aac" && format != "AAC";
  bool c4 = format != "aiff" && format != "AIFF";
  bool c5 = format != "flac" && format != "FLAC";
  if (c1 && c2 && c3 && c4 && c5)
  {
    throw std::out_of_range("Invalid format");
  }
  convertTo(id, newId, format, cht);
}

void lavrentev::convertToWav(std::istream &is, std::ostream &, CCHT &cht)
{
  size_t id, newId;
  std::string format = "wav";
  if (!(is >> id >> newId))
  {
    throw std::invalid_argument("Invalid arguments");
  }
  convertTo(id, newId, format, cht);
}

void lavrentev::convertToMp3(std::istream &is, std::ostream &, CCHT &cht)
{
  size_t id, newId;
  std::string format = "mp3";
  if (!(is >> id >> newId))
  {
    throw std::invalid_argument("Invalid arguments");
  }
  convertTo(id, newId, format, cht);
}

void lavrentev::convertTo(size_t id, size_t newId, std::string format, CCHT &cht)
{
  if (!cht.has(id))
  {
    throw std::out_of_range("No such file");
  }
  if (cht.has(newId))
  {
    throw std::out_of_range("ID already exists");
  }

  Audio &curr = cht[id];
  Audio newAudio;
  newAudio.channelsSize = curr.channelsSize;
  newAudio.bitDepth = curr.bitDepth;
  newAudio.format = format;
  newAudio.name = curr.name;
  newAudio.sampleRate = curr.sampleRate;
  newAudio.channels = new AudioChannel[curr.channelsSize];
  for (size_t i = 0; i < curr.channelsSize; ++i)
  {
    newAudio.channels[i].samples = nullptr;
    newAudio.channels[i].samplesSize = 0;
  }

  try
  {
    for (size_t i = 0; i < curr.channelsSize; ++i)
    {
      size_t samplesCount = curr.channels[i].samplesSize;
      float *newSamples = new float[samplesCount];
      for (size_t j = 0; j < samplesCount; ++j)
      {
        newSamples[j] = curr.channels[i].samples[j];
      }
      newAudio.channels[i].samples = newSamples;
      newAudio.channels[i].samplesSize = samplesCount;
    }
  } catch (...)
  {
    freeAudioMemory(newAudio);
    throw;
  }
  cht[newId] = std::move(newAudio);
}

void lavrentev::pitch(std::istream &is, std::ostream &, CCHT &cht)
{
  size_t id, newId;
  float st;
  if (!(is >> id >> newId >> st))
  {
    throw std::invalid_argument("Invalid arguments");
  }
  if (!cht.has(id))
  {
    throw std::out_of_range("No such file");
  }
  if (cht.has(newId))
  {
    throw std::out_of_range("ID already exists");
  }
  if (abs(st) > 120.0f)
  {
    throw std::out_of_range("Pitch overflow");
  }

  Audio &curr = cht[id];
  float ratio = std::pow(2.0f, st / 12.0f);
  size_t oldSize = curr.channels[0].samplesSize;
  size_t newSize = static_cast<size_t>(std::floor(static_cast<float>(oldSize) / ratio));

  if (newSize <= 1)
  {
    return;
  }

  Audio newAudio;
  newAudio.channelsSize = curr.channelsSize;
  newAudio.sampleRate = curr.sampleRate;
  newAudio.bitDepth = curr.bitDepth;
  newAudio.format = curr.format;
  newAudio.channels = new AudioChannel[newAudio.channelsSize];
  newAudio.name = curr.name;
  for (size_t i = 0; i < curr.channelsSize; ++i)
  {
    newAudio.channels[i].samples = nullptr;
    newAudio.channels[i].samplesSize = 0;
  }

  try
  {
    for (size_t c = 0; c < curr.channelsSize; ++c)
    {
      AudioChannel &channel = curr.channels[c];
      float *oldSamples = channel.samples;
      float *newSamples = new float[newSize];

      for (size_t i = 0; i < newSize; ++i)
      {
        float pos = i * ratio;
        size_t index = static_cast<size_t>(pos);
        float frac = pos - index;
        if (index + 1 < oldSize)
        {
          newSamples[i] = oldSamples[index] * (1.0f - frac) + oldSamples[index + 1] * frac;
        }
        else
        {
          newSamples[i] = oldSamples[index];
        }
      }
      newAudio.channels[c].samples = newSamples;
      newAudio.channels[c].samplesSize = newSize;
    }
  }
  catch (...)
  {
    freeAudioMemory(newAudio);
    throw;
  }

  newAudio.name += "_pitched";
  cht[newId] = std::move(newAudio);
}
