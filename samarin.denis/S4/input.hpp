#ifndef INPUT_HPP
#define INPUT_HPP

#include <iosfwd>
#include "dataset.hpp"

namespace samarin {

  void readDatasets(std::istream & in, DatasetCollection & datasets);

}

#endif
