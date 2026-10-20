#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace strelnikov
{

  class FileSystem;

  class Action
  {
  public:
    virtual ~Action() = default;
    virtual void execute(strelnikov::FileSystem &) = 0;
    virtual void undo(strelnikov::FileSystem &) = 0;
    virtual std::string getCommandName() const = 0;

    using Creator = std::unique_ptr< Action > (*)(const std::vector< std::string > &);

    static std::unordered_map< std::string, Creator > &getRegistry();

    static std::unique_ptr< Action > create(const std::string &, const std::vector< std::string > &);

    template< typename T >
    static void registerCommand(const std::string &name)
    {
      getRegistry()[name] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
        return std::unique_ptr< T >(new T(args));
      };
    }
  };

  class MakeCollectionAction: public Action
  {
    std::string name_;

  public:
    explicit MakeCollectionAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class RmCollectionAction: public Action
  {
    std::string name_;

  public:
    explicit RmCollectionAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class ListCollectionsAction: public Action
  {
  public:
    explicit ListCollectionsAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class AddFileAction: public Action
  {
    std::string collectionName_;
    std::string filePath_;

  public:
    explicit AddFileAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class RmFileAction: public Action
  {
    std::string collectionName_;
    std::string identifier_;
    std::string resolvedPath_;

  public:
    explicit RmFileAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class ShowAction: public Action
  {
    std::string collectionName_;

  public:
    explicit ShowAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class SortAction: public Action
  {
    std::string collectionName_;
    std::string order_;

  public:
    explicit SortAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class CheckFilesAction: public Action
  {
    std::string collectionName_;
    std::string batchAction_;

  public:
    explicit CheckFilesAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class MakeTagAction: public Action
  {
    std::string tagName_;

  public:
    explicit MakeTagAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class RmTagAction: public Action
  {
    std::string tagName_;

  public:
    explicit RmTagAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class GiveTagAction: public Action
  {
    std::string tagName_;
    std::string targetName_;
    std::string resolvedPath_;

  public:
    explicit GiveTagAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class WithdrawTagAction: public Action
  {
    std::string tagName_;
    std::string targetName_;
    std::string resolvedPath_;

  public:
    explicit WithdrawTagAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class FindFileByNameAction: public Action
  {
    std::string fullname_;
    std::string collectionName_;

  public:
    explicit FindFileByNameAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class FindFileByPrefixAction: public Action
  {
    std::string prefix_;
    std::string collectionName_;

  public:
    explicit FindFileByPrefixAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class FindFileByTagAction: public Action
  {
    std::string tagName_;
    std::string collectionName_;

  public:
    explicit FindFileByTagAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class FindTagByNameAction: public Action
  {
    std::string tagName_;

  public:
    explicit FindTagByNameAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class FindTagByPrefixAction: public Action
  {
    std::string prefix_;

  public:
    explicit FindTagByPrefixAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class FindCollectionByNameAction: public Action
  {
    std::string collectionName_;

  public:
    explicit FindCollectionByNameAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class FindCollectionByPrefixAction: public Action
  {
    std::string prefix_;

  public:
    explicit FindCollectionByPrefixAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class UnionCollectionsAction: public Action
  {
    std::string newName_;
    std::string col1_;
    std::string col2_;

  public:
    explicit UnionCollectionsAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class IntersectCollectionsAction: public Action
  {
    std::string newName_;
    std::string col1_;
    std::string col2_;

  public:
    explicit IntersectCollectionsAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  class FilterByTagAction: public Action
  {
    std::string newName_;
    std::string sourceCol_;
    std::string tagName_;

  public:
    explicit FilterByTagAction(const std::vector< std::string > &);
    void execute(strelnikov::FileSystem &) override;
    void undo(strelnikov::FileSystem &) override;
    std::string getCommandName() const override;
  };

  void registerAllActions();

  std::string getFilename(const std::string &);
}

#endif
