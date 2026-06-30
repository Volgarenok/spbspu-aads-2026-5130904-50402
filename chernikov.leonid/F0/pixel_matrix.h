#ifndef PIXEL_MATRIX_H
#define PIXEL_MATRIX_H

#include <vector>
#include <cstddef>
#include <algorithm>
#include <cmath>

template < typename T > class PixelMatrix
{
  std::vector< T > data;
  size_t width, height;

public:
  PixelMatrix():
    width(0),
    height(0)
  {
  }
  PixelMatrix(size_t w, size_t h):
    data(w * h),
    width(w),
    height(h)
  {
  }

  T &operator()(size_t x, size_t y)
  {
    return data[y * width + x];
  }
  const T &operator()(size_t x, size_t y) const
  {
    return data[y * width + x];
  }

  size_t getWidth() const
  {
    return width;
  }
  size_t getHeight() const
  {
    return height;
  }
  std::vector< T > getData() const
  {
    return data;
  }

  T min() const
  {
    return *std::min_element(data.begin(), data.end());
  }
  T max() const
  {
    return *std::max_element(data.begin(), data.end());
  }

  double mean() const
  {
    double sum = 0.0;
    for (const auto &v : data)
      sum += static_cast< double >(v);
    return sum / data.size();
  }

  double stddev() const
  {
    double m = mean();
    double sumSq = 0.0;
    for (const auto &v : data)
    {
      double diff = static_cast< double >(v) - m;
      sumSq += diff * diff;
    }
    return std::sqrt(sumSq / data.size());
  }
};

#endif
