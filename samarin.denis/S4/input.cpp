#include "input.hpp"

#include <istream>
#include <sstream>
#include <string>
#include "dataset.hpp"

void samarin::readDatasets(std::istream & in, DatasetCollection & datasets)
{
  std::string line;
  while (std::getline(in, line)) {
    std::istringstream stream(line);
    std::string name;
    if (!(stream >> name)) {
      continue;
    }
    Dataset dataset;
    long long key = 0;
    std::string value;
    while (stream >> key >> value) {
      dataset.push(key, value);
    }
    datasets.push(name, dataset);
  }
}
