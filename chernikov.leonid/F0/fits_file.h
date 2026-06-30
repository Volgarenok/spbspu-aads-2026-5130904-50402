#ifndef FITS_FILE_H
#define FITS_FILE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <cstdint>
#include <memory>
#include <iostream>
#include "bst.h"
#include "pixel_matrix.h"

enum class PixelType
{
  UINT8,
  INT16,
  INT32,
  FLOAT32,
  FLOAT64,
  NONE
};

class FitsFile
{
public:
  BinarySearchTree header;

  PixelType dataType = PixelType::NONE;
  std::vector< uint8_t > data_uint8;
  std::vector< int16_t > data_int16;
  std::vector< int32_t > data_int32;
  std::vector< float > data_float32;
  std::vector< double > data_float64;

  int width = 0;
  int height = 0;
  int bitpix = 0;
  bool loaded = false;

  FitsFile() = default;

  bool load(const std::string &filename);
  bool save(const std::string &filename) const;
  void printInfo() const;

  double getPixelAsDouble(int x, int y) const;
  void setPixelAsDouble(int x, int y, double value);

  double computeMin() const;
  double computeMax() const;
  double computeMean() const;
  double computeStdDev() const;

  size_t totalPixels() const
  {
    return static_cast< size_t >(width) * height;
  }

  bool calibrate(const FitsFile &darkFrame);

  static std::string trim(const std::string &s);

  template < typename T > static void swapBytes(T &value)
  {
    char *bytes = reinterpret_cast< char * >(&value);
    std::reverse(bytes, bytes + sizeof(T));
  }

private:
  void parseHeaderBlock(const char *block);
  bool readData(std::ifstream &file, int headerBlocks);
  void writeHeader(std::ostream &out) const;
  void writeData(std::ostream &out) const;
};

#endif
