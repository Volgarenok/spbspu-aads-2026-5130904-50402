#ifndef OPERATIONS_CPP
#define OPERATIONS_CPP
#include "operations.hpp"

void permyakov::bin(std::istream & in, permyakov::Manager & man) {
  std::string name;
  float n;
  if (in >> name >> n) {
    if (n < 0 || n > 1) {
      throw std::invalid_argument("expected chance from 0 to 1");
    }
    spc_t &spaces = man.getSpaces();
    evn_t events{};
    events.add("true", n);
    events.add("false", 1 - n);
    if (spaces.has(name)) {
      spaces.drop(name);
    }
    spaces.add(name, events);
    man.saveToFile();
  } else {
    throw std::invalid_argument("expected:<space-name> <chance>");
  }
}

void permyakov::sum(std::istream & in, permyakov::Manager & man) {
  std::string name;
  size_t n;
  if (in >> name >> n) {
    spc_t &spaces = man.getSpaces();
    evn_t source{};
    std::string s, e;
    for(size_t i = 0; i < n; ++i) {
      if (in >> s >> e) {
        float chance = spaces.get(s).get(e);
        if (chance < 0 || chance > 1) {
          throw std::invalid_argument("expected chance from 0 to 1");
        }
        if (source.has(s)) {
          source.get(s) += chance;
          if (source.get(s) > 1) {
            throw std::out_of_range("sum events in one space bigger than 1");
          }
        } else {
          source.add(s, chance);
        }
      } else {
        throw std::invalid_argument("expected:<space-nameX> <event-nameX>");
      }
    }
    permyakov::RobinIter< std::string, float > it(source);
    float sum = (*it).second;
    while(it.hasNext()) {
      ++it;
      sum *= (*it).second;
    }
    evn_t events{};
    events.add("true", sum);
    events.add("false", 1 - sum);
    if (spaces.has(name)) {
      spaces.drop(name);
    }
    spaces.add(name, events);
    man.saveToFile();
  } else {
    throw std::invalid_argument("expected:<space-name> <count>");
  }
}

void permyakov::multi(std::istream & in, permyakov::Manager & man) {
  std::string name;
  size_t n;
  if (in >> name >> n) {
    spc_t &spaces = man.getSpaces();
    float mul = 1;
    std::string s, e;
    for(size_t i = 0; i < n; ++i) {
      if (in >> s >> e) {
        float chance = spaces.get(s).get(e);
        if (chance < 0 || chance > 1) {
          throw std::invalid_argument("expected chance from 0 to 1");
        }
        mul *= chance;
      } else {
        throw std::invalid_argument("expected:<space-nameX> <event-nameX>");
      }
    }
    evn_t events{};
    events.add("true", mul);
    events.add("false", 1 - mul);
    if (spaces.has(name)) {
      spaces.drop(name);
    }
    spaces.add(name, events);
    man.saveToFile();
  } else {
    throw std::invalid_argument("expected:<space-name> <count>");
  }
}

void permyakov::make(std::istream & in, permyakov::Manager & man) {
  std::string name;
  size_t n;
  if (in >> name >> n) {
    evn_t events{};
    std::string e;
    float chance;
    for(size_t i = 0; i < n; ++i) {
      if (in >> e >> chance) {
        if (chance < 0 || chance > 1) {
          throw std::invalid_argument("expected chance from 0 to 1");
        }
        events.add(e, chance);
      } else {
        throw std::invalid_argument("expected:<event-nameX> <chanceX>");
      }
    }
    permyakov::RobinIter< std::string, float > it(events);
    float sum = (*it).second;
    while(it.hasNext()) {
      ++it;
      sum += (*it).second;
    }
    if (sum > 1) {
      throw std::out_of_range("sum events in one space bigger than 1");
    } else if (sum < 1) {
      events.add("none", 1 - sum);
    }
    spc_t &spaces = man.getSpaces();
    if (spaces.has(name)) {
      spaces.drop(name);
    }
    spaces.add(name, events);
    man.saveToFile();
  } else {
    throw std::invalid_argument("expected:<space-name> <count>");
  }
}

void permyakov::construct(std::istream & in, permyakov::Manager & man) {
  std::string name;
  size_t n;
  if (in >> name >> n) {
    spc_t &spaces = man.getSpaces();
    evn_t events{};
    std::string s;
    for(size_t i = 0; i < n; ++i) {
      if (in >> s) {
        float chance = spaces.get(s).get("true");
        if (chance < 0 || chance > 1) {
          throw std::invalid_argument("expected chance from 0 to 1");
        }
        events.add(s, chance);
      } else {
        throw std::invalid_argument("expected:<bin-space-nameX>");
      }
    }
    permyakov::RobinIter< std::string, float > it(events);
    float sum = (*it).second;
    while(it.hasNext()) {
      ++it;
      sum += (*it).second;
    }
    if (sum > 1) {
      throw std::out_of_range("sum events in one space bigger than 1");
    } else if (sum < 1) {
      events.add("none", 1 - sum);
    }
    if (spaces.has(name)) {
      spaces.drop(name);
    }
    spaces.add(name, events);
    man.saveToFile();
  } else {
    throw std::invalid_argument("expected:<space-name> <count>");
  }
}

void permyakov::copy(std::istream & in, permyakov::Manager & man) {
  std::string name1, name2;
  if (in >> name1 >> name2) {
    spc_t &spaces = man.getSpaces();
    permyakov::RobinIter< std::string, float > it(spaces.get(name2));
    evn_t events{};
    events.add((*it).first, (*it).second);
    while (it.hasNext()) {
      ++it;
      events.add((*it).first, (*it).second);
    }
    if (spaces.has(name1)) {
      spaces.drop(name1);
    }
    spaces.add(name1, events);
    man.saveToFile();
  } else {
    throw std::invalid_argument("expected:<space-name1> <space-name2>");
  }
}

void permyakov::reverse(std::istream & in, permyakov::Manager & man) {
  std::string name;
  if (in >> name) {
    spc_t &spaces = man.getSpaces();
    float n = spaces.get(name).get("true");
    evn_t events{};
    events.add("true", 1 - n);
    events.add("false", n);
    if (spaces.has(name)) {
      spaces.drop(name);
    }
    spaces.add(name, events);
    man.saveToFile();
  } else {
    throw std::invalid_argument("expected:<bin-space-name>");
  }
}

void permyakov::get(std::istream & in, permyakov::Manager & man) {
  std::string name;
  if (in >> name) {
    spc_t spaces = man.getSpaces();
    permyakov::RobinIter< std::string, float > it(spaces.get(name));
    std::ostream & out = man.getOutput();
    out << name << ":\n" << (*it).first << ' ' << (*it).second << '\n';
    while (it.hasNext()) {
      ++it;
      out << (*it).first << ' ' << (*it).second << '\n';
    }
  } else {
    throw std::invalid_argument("expected:<space-name>");
  }
}

void permyakov::bayes(std::istream & in, permyakov::Manager & man) {
  std::string name, nameA, nameB;
  float chanceBA;
  if (in >> name >> nameA >> nameB >> chanceBA) {
    spc_t &spaces = man.getSpaces();
    float chanceA = spaces.get(nameA).get("true"), chanceB = spaces.get(nameB).get("true");
    if (chanceA < 0 || chanceA > 1 || chanceB < 0 || chanceB > 1) {
      throw std::invalid_argument("expected chance from 0 to 1");
    }
    float chanceAB = chanceBA * chanceA / chanceB;
    evn_t events{};
    events.add("true", chanceAB);
    events.add("false", 1 - chanceAB);
    if (spaces.has(name)) {
      spaces.drop(name);
    }
    spaces.add(name, events);
    man.saveToFile();
  } else {
    throw std::invalid_argument("expected:<bin-space-name> <bin-space-nameA> <bin-space-nameB> <B|A>");
  }
}

void permyakov::deleteS(std::istream & in, permyakov::Manager & man) {
  std::string name;
  if (in >> name) {
    spc_t &spaces = man.getSpaces();
    spaces.drop(name);
    man.saveToFile();
  } else {
    throw std::invalid_argument("expected:<space-name>");
  }
}

void permyakov::deleteE(std::istream & in, permyakov::Manager & man) {
  std::string s, e;
  if (in >> s >> e) {
    spc_t &spaces = man.getSpaces();
    spaces.get(s).drop(e);
    man.saveToFile();
  } else {
    throw std::invalid_argument("expected:<space-name> <event-name>");
  }
}

#endif
