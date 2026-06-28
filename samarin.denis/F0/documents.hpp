#ifndef DOCUMENTS_HPP
#define DOCUMENTS_HPP

#include <string>
#include "cuckoo_table.hpp"
#include "hashers.hpp"
#include "text_index.hpp"

namespace samarin {

  using Documents = CuckooTable< std::string, TextIndex, StringHash >;

}

#endif
