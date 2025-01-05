#include "free_list.hpp"
#include <cassert>
namespace lmem {

void *&get_next_memptr(void *obj) {
  // (void**)obj是将obj当做指向void*指针p2的指针p1，即将obj指向的内存块当做p2
  // *(void**)obj，就是解引用p1得到p2
  // p2存储在内存块上，不用担心生命周期问题
  return *(void **)obj;
}

void FreeList::push(void *obj) {
  if (obj == nullptr) {
    return;
  }

  get_next_memptr(obj) = list_head_; // 将obj添加到list_head_指向的内存块之前
  list_head_ = obj;
}

void FreeList::push(void *start, void *end, std::size_t nums) {
  get_next_memptr(end) = list_head_;
  list_head_ = start;
}

void *FreeList::pop() {
  assert(list_head_);
  auto res = list_head_;
  list_head_ =
      get_next_memptr(list_head_); // 头删，list_head_指向第一块内存的下一内存
  return res;
}

bool FreeList::empty() const { return list_head_ == nullptr; }
} // namespace lmem