#include "fits_file.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

std::string FitsFile::trim(const std::string &s)
{
  size_t start = s.find_first_not_of(" \t\r\n");
  if (start == std::string::npos)
    return "";
  size_t end = s.find_last_not_of(" \t\r\n");
  return s.substr(start, end - start + 1);
}

void FitsFile::parseHeaderBlock(const char *block)
{
  for (int line = 0; line < 36; line++)
  {
    const char *start = block + line * 80;

    std::string first8(start, 8);
    std::string first3 = trim(first8);

    if (first3 == "END")
      return;
    if (first3.empty() || first3 == "COMMENT" || first3 == "HISTORY")
      continue;

    std::string key = first3;
    std::string rest(start + 8, 72);

    size_t eqPos = rest.find("= ");
    if (eqPos == std::string::npos)
      continue;

    std::string valuePart = rest.substr(eqPos + 2);
    std::string value;
    std::string comment;

    valuePart = trim(valuePart);
    if (!valuePart.empty() && valuePart[0] == '\'')
    {
      size_t closeQuote = valuePart.find('\'', 1);
      if (closeQuote != std::string::npos)
      {
        value = valuePart.substr(1, closeQuote - 1);
        std::string afterQuote = valuePart.substr(closeQuote + 1);
        size_t commentPos = afterQuote.find(" /");
        if (commentPos != std::string::npos)
          comment = trim(afterQuote.substr(commentPos + 2));
      } else
      {
        value = valuePart.substr(1);
      }
    } else
    {
      size_t commentPos = valuePart.find(" /");
      if (commentPos != std::string::npos)
      {
        value = trim(valuePart.substr(0, commentPos));
        comment = trim(valuePart.substr(commentPos + 2));
      } else
      {
        value = trim(valuePart);
      }
      if (value == "T")
        value = "TRUE";
      else if (value == "F")
        value = "FALSE";
    }

    if (!key.empty() && !value.empty())
      header.insert(key, value, comment);
  }
}

bool FitsFile::readData(std::ifstream &file, int headerBlocks)
{
  std::string bitpixStr = header.find("BITPIX");
  std::string naxisStr = header.find("NAXIS");
  std::string naxis1Str = header.find("NAXIS1");
  std::string naxis2Str = header.find("NAXIS2");

  if (bitpixStr.empty())
  {
    std::cerr << "Error: BITPIX not found in header" << std::endl;
    return false;
  }

  bitpix = std::stoi(bitpixStr);
  int naxis = naxisStr.empty() ? 0 : std::stoi(naxisStr);

  if (naxis == 0)
  {
    width = 0;
    height = 0;
    return true;
  }

  if (naxis1Str.empty() || naxis2Str.empty())
  {
    std::cerr << "Error: NAXIS1/NAXIS2 not found" << std::endl;
    return false;
  }

  width = std::stoi(naxis1Str);
  height = std::stoi(naxis2Str);

  size_t totalPixels = static_cast< size_t >(width) * height;
  int bytesPerPixel = std::abs(bitpix) / 8;
  size_t dataBytes = totalPixels * bytesPerPixel;

  if (totalPixels == 0)
    return true;

  file.seekg(headerBlocks * 2880, std::ios::beg);

  try
  {
    switch (bitpix)
    {
    case 8: {
      dataType = PixelType::UINT8;
      data_uint8.resize(totalPixels);
      file.read(reinterpret_cast< char * >(data_uint8.data()), dataBytes);
      break;
    }
    case 16: {
      dataType = PixelType::INT16;
      data_int16.resize(totalPixels);
      file.read(reinterpret_cast< char * >(data_int16.data()), dataBytes);
      for (size_t i = 0; i < data_int16.size(); i++)
        swapBytes(data_int16[i]);
      break;
    }
    case 32: {
      dataType = PixelType::INT32;
      data_int32.resize(totalPixels);
      file.read(reinterpret_cast< char * >(data_int32.data()), dataBytes);
      for (size_t i = 0; i < data_int32.size(); i++)
        swapBytes(data_int32[i]);
      break;
    }
    case -32: {
      dataType = PixelType::FLOAT32;
      data_float32.resize(totalPixels);
      file.read(reinterpret_cast< char * >(data_float32.data()), dataBytes);
      for (size_t i = 0; i < data_float32.size(); i++)
        swapBytes(data_float32[i]);
      break;
    }
    case -64: {
      dataType = PixelType::FLOAT64;
      data_float64.resize(totalPixels);
      file.read(reinterpret_cast< char * >(data_float64.data()), dataBytes);
      for (size_t i = 0; i < data_float64.size(); i++)
        swapBytes(data_float64[i]);
      break;
    }
    default:
      std::cerr << "Error: Unsupported BITPIX: " << bitpix << std::endl;
      return false;
    }
  } catch (const std::exception &e)
  {
    std::cerr << "Error reading data: " << e.what() << std::endl;
    return false;
  }

  double bscale = header.contains("BSCALE") ? std::stod(header.find("BSCALE")) : 1.0;
  double bzero = header.contains("BZERO") ? std::stod(header.find("BZERO")) : 0.0;

  if (bscale != 1.0 || bzero != 0.0)
  {
    for (size_t i = 0; i < totalPixels; i++)
    {
      int x = static_cast< int >(i % width);
      int y = static_cast< int >(i / width);
      double raw = getPixelAsDouble(x, y);
      double calibrated = raw * bscale + bzero;
      setPixelAsDouble(x, y, calibrated);
    }
    header.insert("BSCALE", "1.0", "Calibration applied");
    header.insert("BZERO", "0.0", "Calibration applied");
  }

  return true;
}

bool FitsFile::load(const std::string &filename)
{
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open())
  {
    std::cerr << "Error: Cannot open file: " << filename << std::endl;
    return false;
  }

  char firstBlock[2880];
  file.read(firstBlock, 2880);
  if (file.gcount() < 80)
  {
    std::cerr << "Error: File too small to be a valid FITS" << std::endl;
    return false;
  }

  std::string firstKeyword(firstBlock, 8);
  firstKeyword = trim(firstKeyword);
  if (firstKeyword != "SIMPLE")
    std::cerr << "Warning: File doesn't start with SIMPLE, may not be a valid FITS" << std::endl;

  file.seekg(0, std::ios::beg);

  header = BinarySearchTree();
  data_uint8.clear();
  data_int16.clear();
  data_int32.clear();
  data_float32.clear();
  data_float64.clear();
  dataType = PixelType::NONE;
  width = height = 0;
  bitpix = 0;

  int headerBlocks = 0;
  bool endFound = false;

  while (!endFound && headerBlocks < 1000)
  {
    char block[2880];
    file.read(block, 2880);
    if (file.gcount() != 2880)
    {
      std::cerr << "Error: Incomplete header block" << std::endl;
      return false;
    }
    headerBlocks++;

    for (int i = 0; i < 36; i++)
    {
      std::string lineStart(block + i * 80, 8);
      if (trim(lineStart) == "END")
      {
        endFound = true;
        break;
      }
    }

    parseHeaderBlock(block);
  }

  if (!endFound)
  {
    std::cerr << "Error: END keyword not found in header" << std::endl;
    return false;
  }

  if (!readData(file, headerBlocks))
    return false;

  loaded = true;
  std::cout << "Loaded: " << filename << " (" << width << "x" << height << ", BITPIX=" << bitpix << ")" << std::endl;
  return true;
}

void FitsFile::writeHeader(std::ostream &out) const
{
  std::string headerStr;
  std::vector< std::pair< std::string, std::string > > headerVec = header.toVector();

  for (size_t i = 0; i < headerVec.size(); i++)
  {
    const std::string &key = headerVec[i].first;
    const std::string &value = headerVec[i].second;
    std::string line(80, ' ');

    for (size_t j = 0; j < std::min(key.length(), size_t(8)); j++)
      line[j] = key[j];

    line[8] = '=';
    line[9] = ' ';

    std::string valStr = value;
    bool isString = false;

    if (!valStr.empty())
    {
      bool isNum = true;
      for (size_t j = 0; j < valStr.length(); j++)
      {
        if (j == 0 && (valStr[j] == '-' || valStr[j] == '+'))
          continue;
        if (valStr[j] == '.')
          continue;
        if (valStr[j] >= '0' && valStr[j] <= '9')
          continue;
        if (valStr == "TRUE" || valStr == "FALSE")
        {
          isNum = true;
          break;
        }
        isNum = false;
        break;
      }
      if (!isNum || (valStr.find(' ') != std::string::npos))
        isString = true;
    }

    if (isString)
    {
      line[10] = '\'';
      size_t maxLen = std::min(valStr.length(), size_t(65));
      for (size_t j = 0; j < maxLen; j++)
        line[11 + j] = valStr[j];
      line[11 + std::min(valStr.length(), size_t(65))] = '\'';
    } else
    {
      size_t maxLen = std::min(valStr.length(), size_t(70));
      for (size_t j = 0; j < maxLen; j++)
        line[10 + j] = valStr[j];
    }

    headerStr += line;
  }

  std::string endLine(80, ' ');
  endLine[0] = 'E';
  endLine[1] = 'N';
  endLine[2] = 'D';
  headerStr += endLine;

  while (headerStr.length() % 2880 != 0)
    headerStr += std::string(80, ' ');

  out.write(headerStr.c_str(), headerStr.length());
}

void FitsFile::writeData(std::ostream &out) const
{
  if (totalPixels() == 0)
    return;

  size_t dataBytes = totalPixels() * (std::abs(bitpix) / 8);
  size_t paddedBytes = ((dataBytes + 2879) / 2880) * 2880;

  switch (bitpix)
  {
  case 8:
    out.write(reinterpret_cast< const char * >(data_uint8.data()), dataBytes);
    break;
  case 16: {
    std::vector< int16_t > temp = data_int16;
    for (size_t i = 0; i < temp.size(); i++)
      swapBytes(temp[i]);
    out.write(reinterpret_cast< const char * >(temp.data()), dataBytes);
    break;
  }
  case 32: {
    std::vector< int32_t > temp = data_int32;
    for (size_t i = 0; i < temp.size(); i++)
      swapBytes(temp[i]);
    out.write(reinterpret_cast< const char * >(temp.data()), dataBytes);
    break;
  }
  case -32: {
    std::vector< float > temp = data_float32;
    for (size_t i = 0; i < temp.size(); i++)
      swapBytes(temp[i]);
    out.write(reinterpret_cast< const char * >(temp.data()), dataBytes);
    break;
  }
  case -64: {
    std::vector< double > temp = data_float64;
    for (size_t i = 0; i < temp.size(); i++)
      swapBytes(temp[i]);
    out.write(reinterpret_cast< const char * >(temp.data()), dataBytes);
    break;
  }
  default:
    return;
  }

  size_t padding = paddedBytes - dataBytes;
  if (padding > 0)
  {
    std::vector< char > pad(padding, 0);
    out.write(pad.data(), padding);
  }
}

bool FitsFile::save(const std::string &filename) const
{
  if (!loaded)
  {
    std::cerr << "Error: No file loaded to save" << std::endl;
    return false;
  }

  std::ofstream file(filename, std::ios::binary);
  if (!file.is_open())
  {
    std::cerr << "Error: Cannot create file: " << filename << std::endl;
    return false;
  }

  writeHeader(file);
  writeData(file);

  file.close();
  std::cout << "Saved: " << filename << " (" << width << "x" << height << ")" << std::endl;
  return true;
}

double FitsFile::getPixelAsDouble(int x, int y) const
{
  if (x < 0 || x >= width || y < 0 || y >= height)
    return 0.0;
  size_t idx = static_cast< size_t >(y) * width + x;

  switch (dataType)
  {
  case PixelType::UINT8:
    return static_cast< double >(data_uint8[idx]);
  case PixelType::INT16:
    return static_cast< double >(data_int16[idx]);
  case PixelType::INT32:
    return static_cast< double >(data_int32[idx]);
  case PixelType::FLOAT32:
    return static_cast< double >(data_float32[idx]);
  case PixelType::FLOAT64:
    return data_float64[idx];
  default:
    return 0.0;
  }
}

void FitsFile::setPixelAsDouble(int x, int y, double value)
{
  if (x < 0 || x >= width || y < 0 || y >= height)
    return;
  size_t idx = static_cast< size_t >(y) * width + x;

  switch (dataType)
  {
  case PixelType::UINT8:
    data_uint8[idx] = static_cast< uint8_t >(std::max(0.0, std::min(255.0, value)));
    break;
  case PixelType::INT16:
    data_int16[idx] = static_cast< int16_t >(std::max(-32768.0, std::min(32767.0, value)));
    break;
  case PixelType::INT32:
    data_int32[idx] = static_cast< int32_t >(value);
    break;
  case PixelType::FLOAT32:
    data_float32[idx] = static_cast< float >(value);
    break;
  case PixelType::FLOAT64:
    data_float64[idx] = value;
    break;
  default:
    break;
  }
}

double FitsFile::computeMin() const
{
  if (totalPixels() == 0)
    return 0.0;
  double minVal = getPixelAsDouble(0, 0);
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      minVal = std::min(minVal, getPixelAsDouble(x, y));
  return minVal;
}

double FitsFile::computeMax() const
{
  if (totalPixels() == 0)
    return 0.0;
  double maxVal = getPixelAsDouble(0, 0);
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      maxVal = std::max(maxVal, getPixelAsDouble(x, y));
  return maxVal;
}

double FitsFile::computeMean() const
{
  size_t n = totalPixels();
  if (n == 0)
    return 0.0;
  double sum = 0.0;
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      sum += getPixelAsDouble(x, y);
  return sum / n;
}

double FitsFile::computeStdDev() const
{
  size_t n = totalPixels();
  if (n == 0)
    return 0.0;
  double mean = computeMean();
  double sumSq = 0.0;
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
    {
      double diff = getPixelAsDouble(x, y) - mean;
      sumSq += diff * diff;
    }
  return std::sqrt(sumSq / n);
}

bool FitsFile::calibrate(const FitsFile &darkFrame)
{
  if (!loaded || !darkFrame.loaded)
  {
    std::cerr << "Error: Both main file and dark frame must be loaded" << std::endl;
    return false;
  }

  if (width != darkFrame.width || height != darkFrame.height)
  {
    std::cerr << "Error: Image and dark frame dimensions don't match (" << width << "x" << height << " vs "
              << darkFrame.width << "x" << darkFrame.height << ")" << std::endl;
    return false;
  }

  size_t n = totalPixels();
  std::cout << "Calibrating: subtracting dark frame from " << n << " pixels..." << std::endl;

  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
    {
      double original = getPixelAsDouble(x, y);
      double dark = darkFrame.getPixelAsDouble(x, y);
      setPixelAsDouble(x, y, original - dark);
    }

  header.insert("CALSTAT", "DARK_SUBTRACTED", "Dark frame subtracted");
  header.insert("HISTORY", "Dark frame calibration applied", "");

  std::cout << "Calibration complete. Dark frame subtracted." << std::endl;
  return true;
}

void FitsFile::printInfo() const
{
  if (!loaded)
  {
    std::cout << "No file loaded." << std::endl;
    return;
  }
  std::cout << "\n=== File Information ===" << std::endl;
  std::cout << "  NAXIS1 (width):  " << width << std::endl;
  std::cout << "  NAXIS2 (height): " << height << std::endl;
  std::cout << "  BITPIX:          " << bitpix << std::endl;
  std::cout << "  Data type:       ";
  switch (bitpix)
  {
  case 8:
    std::cout << "unsigned 8-bit integer";
    break;
  case 16:
    std::cout << "16-bit signed integer";
    break;
  case 32:
    std::cout << "32-bit signed integer";
    break;
  case -32:
    std::cout << "32-bit floating point";
    break;
  case -64:
    std::cout << "64-bit floating point";
    break;
  default:
    std::cout << "unknown";
    break;
  }
  std::cout << std::endl;
  std::cout << "  Total pixels:    " << totalPixels() << std::endl;

  if (header.contains("BSCALE") && header.contains("BZERO"))
  {
    std::cout << "  BSCALE:          " << header.find("BSCALE") << std::endl;
    std::cout << "  BZERO:           " << header.find("BZERO") << std::endl;
  }
  std::cout << "========================\n" << std::endl;
}
