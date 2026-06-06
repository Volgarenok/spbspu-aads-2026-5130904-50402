#include "Dictionary.hpp"

sogdanov::Dictionary::Dictionary():
  name_(""),
  data_()
{}

sogdanov::Dictionary::Dictionary(const std::string& name):
  name_(name),
  data_()
{}

const std::string& sogdanov::Dictionary::getName() const
{
  return name_;
}

std::size_t sogdanov::Dictionary::getWordsCount() const
{
  return data_.getSize();
}

void sogdanov::Dictionary::addWord(const std::string& eng, const std::string& rus,
                                   const std::string& pos, const std::string& form)
{
  sogdanov::Vector< sogdanov::Translation >* translations = data_.find(eng);
  sogdanov::Translation new_trans;
  new_trans.russian = rus;
  new_trans.pos = pos;
  new_trans.form = form;

  if (translations != nullptr) {
    translations->pushBack(new_trans);
  } else {
    sogdanov::Vector< sogdanov::Translation > initial_vec;
    initial_vec.pushBack(new_trans);
    data_.insert(eng, initial_vec);
  }
}

void sogdanov::Dictionary::removeWord(const std::string& eng)
{
  data_.remove(eng);
}

const sogdanov::Vector< sogdanov::Translation >* sogdanov::Dictionary::translate(
  const std::string& eng) const
{
  return data_.find(eng);
}

sogdanov::Vector< sogdanov::HashTable< std::string, sogdanov::Vector< sogdanov::Translation > >::Entry >
sogdanov::Dictionary::getEntries() const
{
  return data_.getEntries();
}
