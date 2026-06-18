#include "graph.hpp"

bukreev::Canvas::Canvas(size_t width, size_t height)
{
  initialise(width, height);
}

void bukreev::Canvas::initialise(size_t width, size_t height)
{
  mSymbols = new char[width * height];
  mWidth = width;
  mHeight = height;

  clear();
}

void bukreev::Canvas::setSize(size_t width, size_t height)
{
  delete[] mSymbols;
  initialise(width, height);
}

size_t bukreev::Canvas::getWidth()
{
  return mWidth;
}

size_t bukreev::Canvas::getHeight()
{
  return mHeight;
}

void bukreev::Canvas::drawGraph(const Graph* graph)
{
  for (const point_t pt : graph->points)
  {
    if (pt.x >= mWidth || pt.y >= mHeight)
    {
      continue;
    }

    mSymbols[pt.y * mWidth + pt.x] = graph->symbol;
  }
}

void bukreev::Canvas::display(std::ostream& out)
{
  for (size_t i = mHeight - 1; i > 0; i--)
  {
    for (size_t j = 0; j < mWidth; j++)
    {
      out << mSymbols[i * mWidth + j];
    }
    out << '\n';
  }

  for (size_t j = 0; j < mWidth; j++)
  {
    out << mSymbols[j];
  }
  out << '\n';
}

void bukreev::Canvas::clear()
{
  mSymbols[0] = '+';

  for (size_t i = 1; i < mWidth; i++)
  {
    mSymbols[i] = '-';
  }

  for (size_t i = 1; i < mHeight; i++)
  {
    mSymbols[i * mWidth] = '|';
  }

  for (size_t i = 1; i < mHeight; i++)
  {
    for (size_t j = 1; j < mWidth; j++)
    {
      mSymbols[i * mWidth + j] = ' ';
    }
  }

  mSymbols[(mHeight - 1) * mWidth] = '^';
  mSymbols[mWidth - 1] = '>';
}
