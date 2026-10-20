#include "structs.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#ifdef _WIN32
#include <direct.h>
#define GETCWD _getcwd
#define PATH_SEP '\\'
#else
#include <unistd.h>
#define GETCWD getcwd
#define PATH_SEP '/'
#endif

namespace strelnikov
{
  static std::string getCurrentDir()
  {
    char buffer[4096];
    if (GETCWD(buffer, sizeof(buffer))) {
      return std::string(buffer);
    }
    return "";
  }

  std::string normalizePath(const std::string &path)
  {
    std::string absPath = path;
    if (absPath.empty()) {
      absPath = getCurrentDir();
    } else {
      bool isAbs = false;
#ifdef _WIN32
      if (absPath.size() >= 2 && std::isalpha((unsigned char)absPath[0]) && absPath[1] == ':')
        isAbs = true;
      if (absPath[0] == '\\' || absPath[0] == '/')
        isAbs = true;
#else
      if (absPath[0] == '/')
        isAbs = true;
#endif
      if (!isAbs) {
        std::string cwd = getCurrentDir();
        if (!cwd.empty()) {
          absPath = cwd + PATH_SEP + absPath;
        }
      }
    }

    std::vector< std::string > parts;
    std::string current;
    for (size_t i = 0; i < absPath.size(); ++i) {
      char c = absPath[i];
      if (c == '/' || c == '\\') {
        if (!current.empty()) {
          if (current == ".") {

          } else if (current == "..") {
            if (!parts.empty() && parts.back() != "..") {
              parts.pop_back();
            } else {
              parts.push_back(current);
            }
          } else {
            parts.push_back(current);
          }
          current.clear();
        }
      } else {
        current += c;
      }
    }
    if (!current.empty()) {
      if (current == ".") {

      } else if (current == "..") {
        if (!parts.empty() && parts.back() != "..") {
          parts.pop_back();
        } else {
          parts.push_back(current);
        }
      } else {
        parts.push_back(current);
      }
    }

    std::string result;
#ifdef _WIN32
    if (absPath.size() >= 2 && std::isalpha((unsigned char)absPath[0]) && absPath[1] == ':') {
      result += absPath[0];
      result += ':';
    } else if (absPath.size() >= 2 && (absPath[0] == '\\' || absPath[0] == '/')
               && (absPath[1] == '\\' || absPath[1] == '/')) {
      result += "\\\\";
    } else {
      result += PATH_SEP;
    }
#else
    result += PATH_SEP;
#endif

    for (size_t i = 0; i < parts.size(); ++i) {
      result += parts[i];
      if (i + 1 < parts.size())
        result += PATH_SEP;
    }

    if (result.size() > 1 && result.back() == PATH_SEP) {
      result.pop_back();
    }
    return result;
  }

  Tag::Tag(const std::string &name):
    name_(name)
  {}

  const std::string &Tag::getName() const
  {
    return name_;
  }

  File::File(const std::string &path):
    path_(path),
    tags_()
  {}

  void File::addTag(const std::string &tagName)
  {
    if (hasTag(tagName)) {
      throw std::invalid_argument("tag already exist in file");
    }
    tags_.push_back(tagName);
  }

  void File::withdrawTag(const std::string &tagName)
  {
    auto it = std::find(tags_.begin(), tags_.end(), tagName);
    if (it == tags_.end()) {
      throw std::invalid_argument("tag doesnt exist in file");
    }
    tags_.erase(it);
  }

  bool File::hasTag(const std::string &tagName) const
  {
    return std::find(tags_.begin(), tags_.end(), tagName) != tags_.end();
  }

  const std::string &File::getPath() const
  {
    return path_;
  }

  const std::vector< std::string > &File::getTags() const
  {
    return tags_;
  }

  bool File::validate() const
  {
    std::ifstream f(path_.c_str());
    return f.good();
  }

  void File::show() const
  {
    std::cout << "PATH=\"" << path_ << "\" TAGS=\"";
    for (size_t i = 0; i < tags_.size(); ++i) {
      std::cout << tags_[i];
      if (i + 1 < tags_.size())
        std::cout << " ";
    }

    std::cout << "\"\n";
  }

  void File::setPath(const std::string &newPath)
  {
    path_ = newPath;
  }

  Collection::Collection(const std::string &name):
    name_(name),
    files_(),
    tags_()
  {}

  void Collection::addFile(const std::string &path)
  {
    if (hasFile(path)) {
      throw std::invalid_argument("file already exist in collection");
    }
    files_.push_back(path);
  }

  void Collection::removeFile(const std::string &path)
  {
    auto it = std::find(files_.begin(), files_.end(), path);
    if (it == files_.end()) {
      throw std::invalid_argument("file doesnt exist in collection");
    }
    files_.erase(it);
  }

  bool Collection::hasFile(const std::string &path) const
  {
    return std::find(files_.begin(), files_.end(), path) != files_.end();
  }

  void Collection::addTag(const std::string &tagName)
  {
    if (hasTag(tagName)) {
      throw std::invalid_argument("tag already exist in collection");
    }
    tags_.push_back(tagName);
  }

  void Collection::withdrawTag(const std::string &tagName)
  {
    auto it = std::find(tags_.begin(), tags_.end(), tagName);
    if (it == tags_.end()) {
      throw std::invalid_argument("tag doesnt exist in collection");
    }
    tags_.erase(it);
  }

  bool Collection::hasTag(const std::string &tagName) const
  {
    return std::find(tags_.begin(), tags_.end(), tagName) != tags_.end();
  }

  const std::string &Collection::getName() const
  {
    return name_;
  }

  const std::vector< std::string > &Collection::getFiles() const
  {
    return files_;
  }

  const std::vector< std::string > &Collection::getTags() const
  {
    return tags_;
  }

  bool Collection::empty()
  {
    return files_.empty();
  }

  void Collection::renameFile(const std::string &oldPath, const std::string &newPath)
  {
    auto it = std::find(files_.begin(), files_.end(), oldPath);
    if (it == files_.end()) {
      throw std::invalid_argument("file doesnt exist in collection");
    }
    if (hasFile(newPath)) {
      throw std::invalid_argument("new path already exist in collection");
    }
    *it = newPath;
  }

  void FileSystem::createCollection(const std::string &name)
  {
    if (collections_.count(name)) {
      throw std::invalid_argument("collection already exist");
    }
    collections_.insert({name, std::make_shared< Collection >(name)});
  }

  void FileSystem::removeCollection(const std::string &name)
  {
    auto it = collections_.find(name);
    if (it == collections_.end()) {
      throw std::invalid_argument("collection doesnt exist");
    }
    collections_.erase(it);
  }

  void FileSystem::createFile(const std::string &path)
  {
    std::string normPath = normalizePath(path);
    if (files_.count(normPath)) {
      throw std::invalid_argument("file already exist");
    }
    files_.insert({normPath, std::make_shared< File >(normPath)});
  }

  void FileSystem::removeFile(const std::string &path)
  {
    auto it = files_.find(path);
    if (it == files_.end()) {
      throw std::invalid_argument("file doesnt exist");
    }
    for (auto cit = collections_.begin(); cit != collections_.end(); ++cit) {
      if (cit->second->hasFile(path)) {
        cit->second->removeFile(path);
      }
    }
    files_.erase(it);
  }

  void FileSystem::addFileToCollection(const std::string &collectionName, const std::string &path)
  {
    auto collIt = collections_.find(collectionName);
    if (collIt == collections_.end()) {
      throw std::invalid_argument("collection doesnt exist");
    }
    std::string normPath = normalizePath(path);
    auto fileIt = files_.find(normPath);
    if (fileIt == files_.end()) {
      throw std::invalid_argument("file doesnt exist");
    }

    if (collIt->second->hasFile(normPath)) {
      throw std::invalid_argument("file already exist in collection");
    }

    collIt->second->addFile(normPath);
  }

  void FileSystem::removeFileFromCollection(const std::string &collectionName, const std::string &path)
  {
    auto collIt = collections_.find(collectionName);
    if (collIt == collections_.end()) {
      throw std::invalid_argument("collection doesnt exist");
    }
    std::string normPath = normalizePath(path);
    collIt->second->removeFile(normPath);
    removeFileIfUnreferenced(normPath);
  }

  void FileSystem::createTag(const std::string &name)
  {
    if (tags_.count(name)) {
      throw std::invalid_argument("tag already exist");
    }
    tags_.insert(std::make_pair(name, std::make_shared< Tag >(name)));
  }

  void FileSystem::removeTag(const std::string &name)
  {
    auto it = tags_.find(name);
    if (it == tags_.end()) {
      throw std::invalid_argument("tag doesnt exist");
    }
    for (auto fit = files_.begin(); fit != files_.end(); ++fit) {
      if (fit->second->hasTag(name)) {
        fit->second->withdrawTag(name);
      }
    }
    for (auto cit = collections_.begin(); cit != collections_.end(); ++cit) {
      if (cit->second->hasTag(name)) {
        cit->second->withdrawTag(name);
      }
    }
    tags_.erase(it);
  }

  void FileSystem::giveTagToFile(const std::string &path, const std::string &tagName)
  {
    if (!tags_.count(tagName)) {
      throw std::invalid_argument("tag doesnt exist");
    }
    std::string normPath = normalizePath(path);
    auto fileIt = files_.find(normPath);
    if (fileIt == files_.end()) {
      throw std::invalid_argument("file doesnt exist");
    }
    fileIt->second->addTag(tagName);
  }

  void FileSystem::withdrawTagFromFile(const std::string &path, const std::string &tagName)
  {
    if (!tags_.count(tagName)) {
      throw std::invalid_argument("tag doesnt exist");
    }
    std::string normPath = normalizePath(path);
    auto fileIt = files_.find(normPath);
    if (fileIt == files_.end()) {
      throw std::invalid_argument("file doesnt exist");
    }
    fileIt->second->withdrawTag(tagName);
  }

  void FileSystem::giveTagToCollection(const std::string &collectionName, const std::string &tagName)
  {
    if (!tags_.count(tagName)) {
      throw std::invalid_argument("tag doesnt exist");
    }
    auto collIt = collections_.find(collectionName);
    if (collIt == collections_.end()) {
      throw std::invalid_argument("collection doesnt exist");
    }
    collIt->second->addTag(tagName);
  }

  void FileSystem::withdrawTagFromCollection(const std::string &collectionName, const std::string &tagName)
  {
    if (!tags_.count(tagName)) {
      throw std::invalid_argument("tag doesnt exist");
    }
    auto collIt = collections_.find(collectionName);
    if (collIt == collections_.end()) {
      throw std::invalid_argument("collection doesnt exist");
    }
    collIt->second->withdrawTag(tagName);
  }

  void FileSystem::validateFilesInCollections()
  {
    for (auto it = files_.begin(); it != files_.end();) {
      bool inAnyCollection = isFileInAnyCollection(it->first);
      if (!inAnyCollection) {
        it = files_.erase(it);
      } else {
        ++it;
      }
    }
  }

  const std::unordered_map< std::string, std::shared_ptr< Collection > > &FileSystem::getCollections() const
  {
    return collections_;
  }

  const std::unordered_map< std::string, std::shared_ptr< File > > &FileSystem::getFiles() const
  {
    return files_;
  }

  const std::unordered_map< std::string, std::shared_ptr< Tag > > &FileSystem::getTags() const
  {
    return tags_;
  }

  bool FileSystem::isFileInAnyCollection(const std::string &path) const
  {
    for (auto cit = collections_.begin(); cit != collections_.end(); ++cit) {
      if (cit->second->hasFile(path)) {
        return true;
      }
    }
    return false;
  }

  void FileSystem::removeFileIfUnreferenced(const std::string &path)
  {
    if (!isFileInAnyCollection(path)) {
      auto it = files_.find(path);
      if (it != files_.end()) {
        files_.erase(it);
      }
    }
  }

  bool FileSystem::validate() const
  {

    for (const auto &pair : files_) {
      if (!pair.second->validate()) {
        return false;
      }
    }
    return true;
  }

  void FileSystem::showCollection(const std::string &collectionName) const
  {
    auto it = collections_.find(collectionName);
    if (it == collections_.end()) {
      throw std::invalid_argument("collection doesnt exist");
    }
    for (const auto &path : it->second->getFiles()) {
      auto fileIt = files_.find(path);
      if (fileIt != files_.end()) {
        fileIt->second->show();
      }
    }
  }

  void FileSystem::renameFile(const std::string &oldPath, const std::string &newPath)
  {
    auto it = files_.find(oldPath);
    if (it == files_.end())
      throw std::invalid_argument("file doesnt exist");
    auto filePtr = it->second;
    files_.erase(it);
    filePtr->setPath(newPath);
    files_[newPath] = filePtr;

    for (auto &pair : collections_) {
      if (pair.second->hasFile(oldPath)) {
        pair.second->renameFile(oldPath, newPath);
      }
    }
  }

  void FileSystem::checkFilesInCollection(const std::string &collectionName, const std::string &batchAction)
  {
    auto collIt = collections_.find(collectionName);
    if (collIt == collections_.end()) {
      throw std::invalid_argument("collection doesnt exist");
    }
    auto coll = collIt->second;
    std::vector< std::string > filesToCheck = coll->getFiles();
    bool flag = true;
    for (const auto &path : filesToCheck) {
      auto fileIt = files_.find(path);
      if (fileIt == files_.end()) {
        coll->removeFile(path);
        continue;
      }
      auto file = fileIt->second;
      if (!file->validate()) {
        flag = false;
        std::string action = batchAction;
        if (action.empty()) {

          std::cout << "File \"" << path << "\" is missing or inaccessible.\n";
          std::cout << "Action (rm / mv <new_path> / SKIP): ";
          std::string input;
          if (!std::getline(std::cin, input)) {
            action = "skip";
          } else {
            while (!input.empty() && std::isspace(input.back()))
              input.pop_back();
            action = input;
          }
        }
        if (action == "rm") {
          coll->removeFile(path);
          removeFileIfUnreferenced(path);
        } else if (action.size() > 3 && action.substr(0, 3) == "mv ") {
          std::string newPath = normalizePath(action.substr(3));
          size_t start = newPath.find_first_not_of(" \t");
          if (start != std::string::npos) {
            newPath = newPath.substr(start);
          } else {
            newPath = "";
          }
          if (newPath.empty()) {
            std::cout << "Invalid mv command (path is empty). Skipping.\n";
            continue;
          }
          file->setPath(newPath);
          files_.erase(fileIt);
          files_[newPath] = file;
          try {
            coll->renameFile(path, newPath);
          } catch (const std::exception &e) {

            std::cout << "Warning: " << e.what() << " Reverting changes.\n";
            file->setPath(path);
            files_.erase(newPath);
            files_[path] = file;
          }
        } else {
        }
      }
    }
    if (flag) {

      std::cout << "all files are valid\n";
    }
  }
}
