#ifndef SPAN_LIST_HPP
#define SPAN_LIST_HPP
#include <cstddef>
#include <mutex>
namespace lmem {

using PageID_t = std::size_t;
struct Span {
  PageID_t pageID = 0;
  std::size_t page_nums = 0;
  Span *prev = nullptr;
  Span *next = nullptr;
  void *free_list = nullptr;
  std::size_t use_count = 0;
};

// 双向循环链表
class SpanList {
public:
  struct iterator {
  public:
    Span *&operator++();
    Span *operator++(int);
    Span *&operator--();
    Span *operator--(int);
    Span *&operator*();
    Span *operator->();
    bool operator!=(iterator const &other) const;
    bool operator==(iterator const &other) const;

  private:
    Span *span_;
  };

  SpanList();
  ~SpanList();
  iterator begin();
  iterator end();
  void insert(Span *where, Span *data);
  void push_front(Span *span);
  Span* pop_front();
  void erase(Span *where);
  void lock() const;
  void unlock() const;
  bool empty() const;

  iterator operator++(int);

private:
  void insert_(Span *new_data, Span *prev, Span *next);
  void erase_(Span *prev, Span *next);

private:
  Span *head_;
  mutable std::mutex
      mutex_; // CC中的哈希桶锁，会有多个线程申请相同大小内存，访问同一哈希桶。要加锁
              // TODO换成自旋锁，开销更小
};
} // namespace lmem
#endif