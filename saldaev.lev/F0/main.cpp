#include <fstream>
#include <iostream>
#include "comandHandlers.hpp"

int main()
{
  using Command = void (*)(std::istream &, std::ostream &, saldaev::Baskets &, saldaev::Recipes &);
  using Commands = saldaev::HashTable< std::string, Command, std::hash< std::string >, std::equal_to< std::string > >;

  Commands commands;
  commands.add("help", saldaev::handleHelp);
  commands.add("basket_list", saldaev::handleBasketList);
  commands.add("basket_create", saldaev::handleBasketCreate);
  commands.add("basket_delete", saldaev::handleBasketDelete);
  commands.add("basket_copy", saldaev::handleBasketCopy);
  commands.add("merge", saldaev::handleMerge);
  commands.add("prod_add", saldaev::handleProdAdd);
  commands.add("prod_remove", saldaev::handleProdRemove);
  commands.add("prod_list", saldaev::handleProdList);
  commands.add("recipe_add", saldaev::handleRecipeAdd);
  commands.add("recipe_remove", saldaev::handleRecipeRemove);
  commands.add("show", saldaev::handleShow);
  commands.add("add", saldaev::handleAdd);
  commands.add("remove", saldaev::handleRemove);
  commands.add("cook", saldaev::handleCook);
  commands.add("cookable", saldaev::handleCookable);
  commands.add("show_recipe", saldaev::handleShowRecipe);
  commands.add("need_for", saldaev::handleNeedFor);
  commands.add("where_used", saldaev::handleWhereUsed);
  commands.add("basket_update", saldaev::handleBasketUpdate);

  saldaev::Recipes recipes;

  std::ifstream iFile;
  iFile.open("graphSaveFile.txt");
  if (!iFile.is_open()) {
    std::cerr << " - failed to open graph save file\n";
  } else {
    while (iFile.peek() != '\n') {
      std::string vName;
      size_t n;
      if (!(iFile >> vName >> n)) {
        std::cout << " - graph save file is corrupted\n";
        iFile.setstate(std::ios::failbit);
        recipes.clear();
        break;
      }
      recipes.addVertex({vName, n}, vName);
    }
  }

  std::string vName1;
  std::string vName2;
  size_t weight;
  while (iFile >> vName1) {
    if (!(iFile >> vName2 >> weight)) {
      std::cout << " - graph save file is corrupted\n";
      iFile.setstate(std::ios::failbit);
      recipes.clear();
      break;
    }
    recipes.addEdge(vName1, vName2, weight);
  }

  iFile.close();
  iFile.open("basketsSaveFile.txt");
  if (!iFile.is_open()) {
    std::cerr << " - failed to open baskets save file\n";
  }

  saldaev::Baskets baskets;

  std::string bName;
  while (iFile >> bName) {
    saldaev::Basket b;
    while (iFile.peek() != '\n') {
      std::string item;
      size_t qty;
      if (!(iFile >> item >> qty)) {
        std::cout << " - baskets save file is corrupted\n";
        iFile.setstate(std::ios::failbit);
        baskets.clear();
        break;
      }
      b.add(item, qty);
    }
    baskets.add(bName, b);
  }
  iFile.close();

  std::string cmd = "";
  while (std::cin >> cmd) {
    if (commands.has(cmd)) {
      try {
        commands.get(cmd)(std::cin, std::cout, baskets, recipes);
      } catch (const std::exception &e) {
        std::cout << " - failed to process the command (" << e.what() << ")\n";
      }
      if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    } else {
      std::cout << " - no such command\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::ofstream oFile;
  oFile.open("basketsSaveFile.txt");

  for (std::pair< std::string, saldaev::Basket > &it : baskets) {
    oFile << it.first;
    for (std::pair< std::string, size_t > &iti : it.second) {
      oFile << ' ' << iti.first << ' ' << iti.second;
    }
    oFile << '\n';
  }

  oFile.close();
  oFile.open("graphSaveFile.txt");

  saldaev::Vector< std::string > vers = recipes.vertexIds();
  for (std::string &it : vers) {
    oFile << ' ' << it << ' ' << recipes.getVertexData(it).second;
  }
  oFile << '\n';
  saldaev::Vector< std::pair< std::string, std::string > > edgs = recipes.edgesIds();
  for (std::pair< std::string, std::string > &it : edgs) {
    oFile << ' ' << it.first << ' ' << it.second << ' ' << recipes.getEdgeData(it.first, it.second);
  }
  oFile.close();
}
