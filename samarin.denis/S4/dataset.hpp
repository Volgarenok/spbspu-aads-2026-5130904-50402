#ifndef DATASET_HPP
#define DATASET_HPP

#include <string>
#include "tree.hpp"

namespace samarin {

  using Dataset = BSTree< long long, std::string >;
  using DatasetCollection = BSTree< std::string, Dataset >;

}

#endif
