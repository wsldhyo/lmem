#ifndef THREAD_CACHE_HPP
#define THREAD_CACHE_HPP

#include <cstddef>

#include "free_list.hpp"
#include "global_var.hpp"
namespace lmem {
class ThreadCache {
public:
  void *allocate(std::size_t size);
  void deallocate(void *obj, std::size_t size);
  //
  // @brief 向CC申请空间
  //     当TC中空间不足时，TC需要向CC申请相应大小的内存备用。为了避免频繁项CC申请，
  //     TC每次都比前一次多申请内存，类似std::vector的扩容策略
  // @param [in] hash_index 申请内存块对应的哈希桶索引
  // @param [in] aligned_size 对齐后的内存块大小
  // @preturn CC返回的内存块首地址
  //
  void *fetch_mem_from_cc(std::size_t hash_index, std::size_t aligned_size);
  void return_mem_to_cc(FreeList& free_list, std::size_t aligned_size);
  void expand_fetch_nums(std::size_t index);

private:
  FreeList free_lists_[FREE_LIST_NUM];
  std::size_t fetch_mem_nums_[FREE_LIST_NUM] = {0};
};

inline extern thread_local ThreadCache *g_thread_cache;
} // namespace lmem

#endif