#include "dictionary.hpp"
#include <memory>
#include <utility>

namespace
{
  std::size_t countGroupWords(const pozdnyakov::List< pozdnyakov::detail::PosGroup > &groups)
  {
    std::size_t total = 0;
    for (auto group = groups.cbegin(); group != groups.cend(); ++group) {
      for (auto word = (*group).rusWords_.cbegin(); word != (*group).rusWords_.cend(); ++word) {
        ++total;
      }
    }
    return total;
  }

  std::size_t countGroups(const pozdnyakov::List< pozdnyakov::detail::PosGroup > &groups)
  {
    std::size_t total = 0;
    for (auto group = groups.cbegin(); group != groups.cend(); ++group) {
      ++total;
    }
    return total;
  }

  pozdnyakov::detail::WordNode *cloneNode(const pozdnyakov::detail::WordNode *node)
  {
    if (node == nullptr) {
      return nullptr;
    }
    pozdnyakov::detail::WordNode * const copy = new pozdnyakov::detail::WordNode(node->engWord_);
    try {
      copy->groups_ = node->groups_;
      copy->height_ = node->height_;
      copy->left_ = cloneNode(node->left_);
      copy->right_ = cloneNode(node->right_);
    } catch (...) {
      delete copy;
      throw;
    }
    return copy;
  }

  void appendUnique(pozdnyakov::List< std::string > &words, const std::string &value, bool &added)
  {
    added = false;
    pozdnyakov::LIter< std::string > tail = words.end();
    for (auto it = words.begin(); it != words.end(); ++it) {
      if (*it == value) {
        return;
      }
      tail = it;
    }
    if (tail == words.end()) {
      words.pushFront(value);
    } else {
      words.insertAfter(tail, value);
    }
    added = true;
  }
}

pozdnyakov::detail::WordNode::WordNode(const std::string &engWord):
  engWord_(engWord),
  groups_(),
  height_(1),
  left_(nullptr),
  right_(nullptr)
{}

pozdnyakov::detail::WordNode::~WordNode()
{
  delete left_;
  delete right_;
}

pozdnyakov::AvlDictionary::AvlDictionary() noexcept:
  root_(nullptr),
  size_(0),
  translationCount_(0)
{}

pozdnyakov::AvlDictionary::AvlDictionary(const AvlDictionary &other):
  root_(cloneNode(other.root_)),
  size_(other.size_),
  translationCount_(other.translationCount_)
{}

pozdnyakov::AvlDictionary::AvlDictionary(AvlDictionary &&other) noexcept:
  root_(other.root_),
  size_(other.size_),
  translationCount_(other.translationCount_)
{
  other.root_ = nullptr;
  other.size_ = 0;
  other.translationCount_ = 0;
}

pozdnyakov::AvlDictionary::~AvlDictionary()
{
  delete root_;
}

pozdnyakov::AvlDictionary &pozdnyakov::AvlDictionary::operator=(AvlDictionary other) noexcept
{
  swap(other);
  return *this;
}

void pozdnyakov::AvlDictionary::swap(AvlDictionary &other) noexcept
{
  std::swap(root_, other.root_);
  std::swap(size_, other.size_);
  std::swap(translationCount_, other.translationCount_);
}

std::size_t pozdnyakov::AvlDictionary::getHeight(const detail::WordNode *node) const noexcept
{
  return (node == nullptr) ? 0 : node->height_;
}

int pozdnyakov::AvlDictionary::getBalanceFactor(const detail::WordNode *node) const noexcept
{
  if (node == nullptr) {
    return 0;
  }
  return static_cast< int >(getHeight(node->right_)) - static_cast< int >(getHeight(node->left_));
}

void pozdnyakov::AvlDictionary::updateHeight(detail::WordNode *node) noexcept
{
  if (node != nullptr) {
    const std::size_t leftHeight = getHeight(node->left_);
    const std::size_t rightHeight = getHeight(node->right_);
    node->height_ = 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
  }
}

pozdnyakov::detail::WordNode *pozdnyakov::AvlDictionary::rotateRight(detail::WordNode *node) noexcept
{
  detail::WordNode * const leftChild = node->left_;
  node->left_ = leftChild->right_;
  leftChild->right_ = node;
  updateHeight(node);
  updateHeight(leftChild);
  return leftChild;
}

pozdnyakov::detail::WordNode *pozdnyakov::AvlDictionary::rotateLeft(detail::WordNode *node) noexcept
{
  detail::WordNode * const rightChild = node->right_;
  node->right_ = rightChild->left_;
  rightChild->left_ = node;
  updateHeight(node);
  updateHeight(rightChild);
  return rightChild;
}

pozdnyakov::detail::WordNode *pozdnyakov::AvlDictionary::balance(detail::WordNode *node) noexcept
{
  updateHeight(node);
  const int balanceFactor = getBalanceFactor(node);

  if (balanceFactor > 1) {
    if (getBalanceFactor(node->right_) < 0) {
      node->right_ = rotateRight(node->right_);
    }
    return rotateLeft(node);
  }
  if (balanceFactor < -1) {
    if (getBalanceFactor(node->left_) > 0) {
      node->left_ = rotateLeft(node->left_);
    }
    return rotateRight(node);
  }
  return node;
}

pozdnyakov::detail::WordNode *pozdnyakov::AvlDictionary::findNode(detail::WordNode *node,
  const std::string &engWord) const noexcept
{
  detail::WordNode *current = node;
  while (current != nullptr) {
    if (engWord < current->engWord_) {
      current = current->left_;
    } else if (current->engWord_ < engWord) {
      current = current->right_;
    } else {
      return current;
    }
  }
  return nullptr;
}

pozdnyakov::detail::WordNode *pozdnyakov::AvlDictionary::findMin(detail::WordNode *node) const noexcept
{
  detail::WordNode *current = node;
  while (current != nullptr && current->left_ != nullptr) {
    current = current->left_;
  }
  return current;
}

void pozdnyakov::AvlDictionary::addToNode(detail::WordNode *node, const std::string &rusWord,
  const std::string &partOfSpeech)
{
  detail::PosGroup *target = nullptr;
  LIter< detail::PosGroup > tail = node->groups_.end();
  for (auto it = node->groups_.begin(); it != node->groups_.end(); ++it) {
    if ((*it).partOfSpeech_ == partOfSpeech) {
      target = std::addressof(*it);
      break;
    }
    tail = it;
  }

  if (target != nullptr) {
    bool added = false;
    appendUnique(target->rusWords_, rusWord, added);
    if (added) {
      ++translationCount_;
    }
    return;
  }

  detail::PosGroup group;
  group.partOfSpeech_ = partOfSpeech;
  group.rusWords_.pushFront(rusWord);
  if (tail == node->groups_.end()) {
    node->groups_.pushFront(std::move(group));
  } else {
    node->groups_.insertAfter(tail, std::move(group));
  }
  ++translationCount_;
}

pozdnyakov::detail::WordNode *pozdnyakov::AvlDictionary::insertWord(detail::WordNode *node,
  const std::string &engWord, const std::string &rusWord, const std::string &partOfSpeech)
{
  if (node == nullptr) {
    detail::WordNode * const newNode = new detail::WordNode(engWord);
    try {
      addToNode(newNode, rusWord, partOfSpeech);
    } catch (...) {
      delete newNode;
      throw;
    }
    ++size_;
    return newNode;
  }

  if (engWord < node->engWord_) {
    node->left_ = insertWord(node->left_, engWord, rusWord, partOfSpeech);
  } else if (node->engWord_ < engWord) {
    node->right_ = insertWord(node->right_, engWord, rusWord, partOfSpeech);
  } else {
    addToNode(node, rusWord, partOfSpeech);
    return node;
  }
  return balance(node);
}

pozdnyakov::detail::WordNode *pozdnyakov::AvlDictionary::removeWordNode(detail::WordNode *node,
  const std::string &engWord)
{
  if (node == nullptr) {
    return nullptr;
  }

  if (engWord < node->engWord_) {
    node->left_ = removeWordNode(node->left_, engWord);
  } else if (node->engWord_ < engWord) {
    node->right_ = removeWordNode(node->right_, engWord);
  } else {
    detail::WordNode * const leftChild = node->left_;
    detail::WordNode * const rightChild = node->right_;

    if (rightChild == nullptr || leftChild == nullptr) {
      node->left_ = nullptr;
      node->right_ = nullptr;
      delete node;
      return (rightChild == nullptr) ? leftChild : rightChild;
    }

    detail::WordNode * const minNode = findMin(rightChild);
    node->engWord_ = minNode->engWord_;
    node->groups_ = minNode->groups_;
    node->right_ = removeWordNode(node->right_, node->engWord_);
  }
  return balance(node);
}

void pozdnyakov::AvlDictionary::addWord(const std::string &engWord, const std::string &rusWord,
  const std::string &partOfSpeech)
{
  root_ = insertWord(root_, engWord, rusWord, partOfSpeech);
}

pozdnyakov::AvlDictionary::AddTransStatus pozdnyakov::AvlDictionary::addTranslation(const std::string &engWord,
  const std::string &rusWord)
{
  detail::WordNode * const node = findNode(root_, engWord);
  if (node == nullptr) {
    return AddTransStatus::NoWord;
  }
  if (countGroups(node->groups_) != 1) {
    return AddTransStatus::AmbiguousPos;
  }
  addToNode(node, rusWord, node->groups_.front().partOfSpeech_);
  return AddTransStatus::Ok;
}

bool pozdnyakov::AvlDictionary::removeWord(const std::string &engWord)
{
  const detail::WordNode * const node = findNode(root_, engWord);
  if (node == nullptr) {
    return false;
  }
  translationCount_ -= countGroupWords(node->groups_);
  --size_;
  root_ = removeWordNode(root_, engWord);
  return true;
}

bool pozdnyakov::AvlDictionary::removeTranslation(const std::string &engWord, const std::string &rusWord)
{
  detail::WordNode * const node = findNode(root_, engWord);
  if (node == nullptr) {
    return false;
  }

  bool removed = false;
  for (auto group = node->groups_.begin(); group != node->groups_.end(); ++group) {
    List< std::string > &words = (*group).rusWords_;
    auto prev = words.end();
    auto word = words.begin();
    while (word != words.end()) {
      if (*word == rusWord) {
        words.eraseAfter(prev);
        --translationCount_;
        removed = true;
        word = prev;
        ++word;
      } else {
        prev = word;
        ++word;
      }
    }
  }

  if (!removed) {
    return false;
  }

  auto prevGroup = node->groups_.end();
  auto group = node->groups_.begin();
  while (group != node->groups_.end()) {
    if ((*group).rusWords_.empty()) {
      node->groups_.eraseAfter(prevGroup);
      group = prevGroup;
      ++group;
    } else {
      prevGroup = group;
      ++group;
    }
  }

  if (node->groups_.empty()) {
    --size_;
    root_ = removeWordNode(root_, engWord);
  }
  return true;
}

bool pozdnyakov::AvlDictionary::contains(const std::string &engWord) const
{
  return findNode(root_, engWord) != nullptr;
}

bool pozdnyakov::AvlDictionary::containsTranslation(const std::string &engWord, const std::string &rusWord,
  const std::string &partOfSpeech) const
{
  const detail::WordNode * const node = findNode(root_, engWord);
  if (node == nullptr) {
    return false;
  }
  for (auto group = node->groups_.cbegin(); group != node->groups_.cend(); ++group) {
    if ((*group).partOfSpeech_ != partOfSpeech) {
      continue;
    }
    for (auto word = (*group).rusWords_.cbegin(); word != (*group).rusWords_.cend(); ++word) {
      if (*word == rusWord) {
        return true;
      }
    }
  }
  return false;
}

const pozdnyakov::List< pozdnyakov::detail::PosGroup > *pozdnyakov::AvlDictionary::groupsOf(
  const std::string &engWord) const
{
  const detail::WordNode * const node = findNode(root_, engWord);
  if (node == nullptr) {
    return nullptr;
  }
  return std::addressof(node->groups_);
}

std::size_t pozdnyakov::AvlDictionary::size() const noexcept
{
  return size_;
}

std::size_t pozdnyakov::AvlDictionary::translationCount() const noexcept
{
  return translationCount_;
}

bool pozdnyakov::AvlDictionary::empty() const noexcept
{
  return size_ == 0;
}
