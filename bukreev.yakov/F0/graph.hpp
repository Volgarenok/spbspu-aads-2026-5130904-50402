#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <list.hpp>
#include <unordered_map>
#include <string>
#include <iostream>

namespace bukreev
{
  struct point_t
  {
    size_t x;
    size_t y;
  };

  struct Graph
  {
    List< point_t > points;
    char symbol;
  };

  class Canvas
  {
  public:
    Canvas(size_t width, size_t height);
    size_t getWidth();
    size_t getHeight();
    void setSize(size_t width, size_t height);
    void drawGraph(const Graph* graph);
    void display(std::ostream& out);
    void clear();

  private:
    void initialise(size_t width, size_t height);

  private:
    char* mSymbols;
    size_t mWidth;
    size_t mHeight;
  };
}

#endif
