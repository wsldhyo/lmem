#include "page_cache.hpp"
#include "global_var.hpp"
#include "span_list.hpp"
#include "sys_alloc.hpp"
#include <cassert>

namespace lmem {

PageCache *PageCache::get_instance() {
  if (instance_ == nullptr) {
    instance_ = new PageCache();
  }
  return instance_;
}

Span *PageCache::new_raw_span(std::size_t k) {
  assert(k > 0 && k < MAX_PAGE_NUM);

  if (!span_lists_[k].empty()) {
    Span *span = span_lists_[k].pop_front();
    for (int i = 0; i < span->page_num; ++i) {
      pageID_span_map_[span->pageID + i] = span;
    }
    return span;
  }

  for (int i = 0; i < MAX_PAGE_NUM; ++i) {
    if (!span_lists_[i].empty()) {
      Span *i_span = span_lists_[i].pop_front();
      Span *k_span = new Span;
      k_span->pageID = i_span->pageID;
      k_span->page_num = k;

      i_span->pageID += k;
      i_span->page_num -= k;
      span_lists_[i_span->page_num].push_front(i_span);

      pageID_span_map_[i_span->pageID] = i_span;
      pageID_span_map_[i_span->pageID + i_span->page_num - 1] = i_span;
      for (int i = 0; i < k_span->page_num; ++i) {
        pageID_span_map_[k_span->pageID + i] = k_span;
      }
      span_lists_[i_span->page_num].push_front(i_span);      
      return k_span;
    }
  }

  void *ptr = system_allocate(MAX_PAGE_NUM - 1);
  Span *big_span = new Span();
  big_span->pageID = ((PageID_t)ptr) >> PAGE_SHIFT;
  big_span->page_num = MAX_PAGE_NUM - 1;
  span_lists_[MAX_PAGE_NUM - 1].push_front(big_span);
  return new_raw_span(k);
}


    void PageCache::return_span_to_pc(Span* span){
      while (true) {
        PageID_t left_ID = span->pageID - 1;
        auto res = pageID_span_map_.find(left_ID);

        if (res == pageID_span_map_.end()) {
          break;
        } 

        Span* left_span = res->second;
        if (left_span->used_by_cc) {
            break;
        }
        if (left_span->page_num + span->page_num > MAX_PAGE_NUM - 1) {
          break;
        }
        span->pageID = left_span->pageID;
        span->page_num += left_span->page_num;
        span_lists_[left_span->page_num].erase(left_span);
        delete left_span;

      }
      while (true) {
        PageID_t right_ID = span->pageID + span->page_num;
        auto res = pageID_span_map_.find(right_ID);

        if (res == pageID_span_map_.end()) {
          break;
        } 

        Span* right_span = res->second;
        if (right_span->used_by_cc) {
            break;
        }
        if (right_span->page_num + span->page_num > MAX_PAGE_NUM - 1) {
          break;
        }
        span->page_num += right_span->page_num;
        span_lists_[right_span->page_num].erase(right_span);
        delete right_span;

      }
      span_lists_[span->page_num].push_front(span);
      span->used_by_cc = false;
      pageID_span_map_[span->pageID] = span;
      pageID_span_map_[span->pageID + span->page_num] = span;

    }

void PageCache::lock() const { page_mutex_.lock(); }

void PageCache::unlock() const { page_mutex_.unlock(); }
Span *PageCache::map_obj_to_span(void *obj) const {
  PageID_t pageID = ((PageID_t)obj) >> PAGE_SHIFT;
  auto res = pageID_span_map_.find(pageID);
  if (res != pageID_span_map_.end()) {
    return res->second;
  } else {
    assert(false);
    return nullptr;
  }
}
} // namespace lmem