#include "span_list.hpp"
#include <cassert>

namespace lmem {
SpanList::SpanList() {
  head_ = new Span();
  head_->next = head_;
  head_->prev = head_;
}

SpanList::~SpanList() { delete head_; }

// 插入到where之前
void SpanList::insert(Span *where, Span *data) {
  assert(where);
  assert(data);
  insert_(data, where->prev, where);
}

void SpanList::insert_(Span *new_data, Span *prev, Span *next) {
  new_data->next = next;
  new_data->prev = prev;
  prev->next = new_data;
  next->prev = new_data;
}

void SpanList::erase_(Span *prev, Span *next) {
  prev->next = next;
  next->prev = prev;
}

void SpanList::erase(Span *where) {
  assert(where != head_); // 不能删除头节点
  if (where == nullptr) {
    return;
  }
  erase_(where->prev, where->next);
}
} // namespace lmem