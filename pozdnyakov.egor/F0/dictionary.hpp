#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <cstddef>
#include <string>
#include "../common/list.hpp"

namespace pozdnyakov
{
  namespace detail
  {

    struct PosGroup
    {
      std::string partOfSpeech_;
      List< std::string > rusWords_;
    };

    class WordNode
    {
    public:
      std::string engWord_;
      List< PosGroup > groups_;
      std::size_t height_;
      WordNode *left_;
      WordNode *right_;

      explicit WordNode(const std::string &engWord);
      ~WordNode();
    };

  }

  class AvlDictionary
  {
  public:
    enum class AddTransStatus
    {
      Ok,
      NoWord,
      AmbiguousPos
    };

    AvlDictionary() noexcept;
    AvlDictionary(const AvlDictionary &other);
    AvlDictionary(AvlDictionary &&other) noexcept;
    ~AvlDictionary();

    AvlDictionary &operator=(AvlDictionary other) noexcept;

    void swap(AvlDictionary &other) noexcept;

    void addWord(const std::string &engWord, const std::string &rusWord, const std::string &partOfSpeech);
    AddTransStatus addTranslation(const std::string &engWord, const std::string &rusWord);
    bool removeWord(const std::string &engWord);
    bool removeTranslation(const std::string &engWord, const std::string &rusWord);

    bool contains(const std::string &engWord) const;
    bool containsTranslation(const std::string &engWord, const std::string &rusWord,
      const std::string &partOfSpeech) const;
    const List< detail::PosGroup > *groupsOf(const std::string &engWord) const;

    std::size_t size() const noexcept;
    std::size_t translationCount() const noexcept;
    bool empty() const noexcept;

    template< class F >
    F traverse(F f) const
    {
      traverseNode(root_, f);
      return f;
    }

  private:
    detail::WordNode *root_;
    std::size_t size_;
    std::size_t translationCount_;

    std::size_t getHeight(const detail::WordNode *node) const noexcept;
    int getBalanceFactor(const detail::WordNode *node) const noexcept;
    void updateHeight(detail::WordNode *node) noexcept;

    detail::WordNode *rotateRight(detail::WordNode *node) noexcept;
    detail::WordNode *rotateLeft(detail::WordNode *node) noexcept;
    detail::WordNode *balance(detail::WordNode *node) noexcept;

    detail::WordNode *insertWord(detail::WordNode *node, const std::string &engWord,
      const std::string &rusWord, const std::string &partOfSpeech);
    detail::WordNode *removeWordNode(detail::WordNode *node, const std::string &engWord);
    detail::WordNode *findMin(detail::WordNode *node) const noexcept;
    detail::WordNode *findNode(detail::WordNode *node, const std::string &engWord) const noexcept;

    void addToNode(detail::WordNode *node, const std::string &rusWord, const std::string &partOfSpeech);

    template< class F >
    void traverseNode(const detail::WordNode *node, F &f) const
    {
      if (node == nullptr) {
        return;
      }
      traverseNode(node->left_, f);
      f(node->engWord_, node->groups_);
      traverseNode(node->right_, f);
    }
  };

}

#endif
