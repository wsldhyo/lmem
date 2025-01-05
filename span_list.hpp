#ifndef SPAN_LIST_HPP
#define SPAN_LIST_HPP
#include <cstddef>
#include <mutex>
namespace lmem {

struct Span {
  std::size_t pageID = 0;
  std::size_t page_nums = 0;
  Span *prev = nullptr;
  Span *next = nullptr;
  void *free_list = nullptr;
  std::size_t use_count = 0;
};

// 双向循环链表
class SpanList {
public:
  SpanList();
  ~SpanList();

  void insert(Span *where, Span *data);
  void erase(Span *where);

private:
  void insert_(Span *new_data, Span *prev, Span *next);
  void erase_(Span *prev, Span *next);

private:
  Span *head_;
  std::mutex
      mutex_; // CC中的哈希桶锁，会有多个线程申请相同大小内存，访问同一哈希桶。要加锁
              // TODO换成自旋锁，开销更小
};
} // namespace lmem
#endif