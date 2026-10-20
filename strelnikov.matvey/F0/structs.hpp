#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace strelnikov
{

  class Tag
  {
  public:
    explicit Tag(const std::string &name);

    const std::string &getName() const;

  private:
    std::string name_;
  };

  class File
  {
  public:
    explicit File(const std::string &);

    void addTag(const std::string &);
    void withdrawTag(const std::string &);
    bool hasTag(const std::string &) const;

    const std::string &getPath() const;
    void setPath(const std::string &);
    const std::vector< std::string > &getTags() const;

    bool validate() const;
    void show() const;

  private:
    std::string path_;
    std::vector< std::string > tags_;
  };

  class Collection
  {
  public:
    explicit Collection(const std::string &);

    void addFile(const std::string &);
    void removeFile(const std::string &);
    bool hasFile(const std::string &) const;

    void addTag(const std::string &);
    void withdrawTag(const std::string &);
    bool hasTag(const std::string &) const;

    bool empty();

    void renameFile(const std::string &, const std::string &);

    const std::string &getName() const;
    const std::vector< std::string > &getFiles() const;
    const std::vector< std::string > &getTags() const;

  private:
    std::string name_;
    std::vector< std::string > files_;
    std::vector< std::string > tags_;
  };

  class FileSystem
  {
  public:
    void createCollection(const std::string &);
    void removeCollection(const std::string &);

    void createFile(const std::string &);
    void removeFile(const std::string &);

    void addFileToCollection(const std::string &, const std::string &);
    void removeFileFromCollection(const std::string &, const std::string &);

    void createTag(const std::string &);
    void removeTag(const std::string &);

    void giveTagToFile(const std::string &, const std::string &);
    void withdrawTagFromFile(const std::string &, const std::string &);

    void giveTagToCollection(const std::string &, const std::string &);
    void withdrawTagFromCollection(const std::string &, const std::string &);

    void validateFilesInCollections();

    bool validate() const;
    void showCollection(const std::string &) const;

    void checkFilesInCollection(const std::string &, const std::string & = "");
    void renameFile(const std::string &, const std::string &);

    const std::unordered_map< std::string, std::shared_ptr< Collection > > &getCollections() const;
    const std::unordered_map< std::string, std::shared_ptr< File > > &getFiles() const;
    const std::unordered_map< std::string, std::shared_ptr< Tag > > &getTags() const;

  private:
    bool isFileInAnyCollection(const std::string &) const;

    void removeFileIfUnreferenced(const std::string &);

    std::unordered_map< std::string, std::shared_ptr< Collection > > collections_;
    std::unordered_map< std::string, std::shared_ptr< File > > files_;
    std::unordered_map< std::string, std::shared_ptr< Tag > > tags_;
  };

  std::string normalizePath(const std::string &);
}

#endif
