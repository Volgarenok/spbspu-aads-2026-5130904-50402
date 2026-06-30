#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include <vector>
#include <cmath>
#include <iostream>

struct Source
{
  int x, y;
  double brightness;
};

struct QuadNode
{
  int x, y;
  int width, height;
  double avgBrightness;
  bool isLeaf;
  QuadNode *children[4];

  QuadNode(int x_, int y_, int w_, int h_):
    x(x_),
    y(y_),
    width(w_),
    height(h_),
    avgBrightness(0.0),
    isLeaf(true)
  {
    for (int i = 0; i < 4; i++)
      children[i] = nullptr;
  }

  ~QuadNode()
  {
    for (int i = 0; i < 4; i++)
      delete children[i];
  }
};

class QuadTree
{
public:
  typedef double (*PixelGetter)(void *context, int x, int y);

private:
  QuadNode *root;
  static const int MIN_LEAF_SIZE = 16;

  QuadNode *build(void *context, PixelGetter getter, int x, int y, int w, int h);
  void findSourcesRec(QuadNode *node, void *context, PixelGetter getter, double threshold,
                      std::vector< Source > &result);
  double getPixelRec(QuadNode *node, void *context, PixelGetter getter, int px, int py);

public:
  QuadTree():
    root(nullptr)
  {
  }
  ~QuadTree()
  {
    delete root;
  }

  void build(void *context, PixelGetter getter, int width, int height);
  double getPixel(void *context, PixelGetter getter, int x, int y);
  std::vector< Source > findSources(void *context, PixelGetter getter, double threshold);
  void printStats() const;
};

#endif
