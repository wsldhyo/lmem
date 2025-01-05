#include "thread_cache.hpp"
#include "central_cache.hpp"
#include "global_var.hpp"
#include "size_class.hpp"
#include <cassert>

namespace lmem {

inline thread_local ThreadCache *g_thread_cache = nullptr;

void *ThreadCache::allocate(std::size_t size) {
  auto aligned_size = SizeClass::round_up(size);
  assert(aligned_size <= MAX_TC_BLOCK_SIZE);
  auto hash_index = SizeClass::get_hash_index(size);
  if (!free_lists_[hash_index].empty()) {
    return free_lists_[hash_index].pop();
  } else {
    return fetch_mem_from_cc(hash_index, aligned_size);
  }
}

void ThreadCache::deallocate(void *obj, std::size_t aligned_size) {
  if (obj == nullptr) {
    return;
  }
  assert(aligned_size <= MAX_TC_BLOCK_SIZE);
  auto hash_index = SizeClass::get_hash_index(aligned_size);
  free_lists_[hash_index].push(obj);
}

void *ThreadCache::fetch_mem_from_cc(std::size_t hash_index,
                                     std::size_t aligned_size) {
  if (fetch_mem_nums_[hash_index] == 0) {
    fetch_mem_nums_[hash_index] = 1;
  }
  auto apply_nums =
      SizeClass::cacl_apply_mem_nums(aligned_size, fetch_mem_nums_[hash_index]);
  // 如果单次申请数量没有达到上限
  // 则增加fetch_mem_nums_[hash_index]
  // 以便让下次申请相同大小内存时，向CC申请更多块内存
  if (apply_nums == fetch_mem_nums_[hash_index]) {
    expand_fetch_nums(hash_index);
  }

  CentralCache *CC = CentralCache::get_instance();
  void *start;
  void *end;
  auto actual_num = CC->fetch_batch_mems(start, end, apply_nums, aligned_size);
  if (actual_num == 1) {
    assert(start == end);
  } else {
    free_lists_[hash_index].push(start, end, actual_num);
  }
  return start;
}

void ThreadCache::expand_fetch_nums(std::size_t index) {
  assert(index <= FREE_LIST_NUM);
  if (fetch_mem_nums_[index] < 2) {
    ++fetch_mem_nums_[index];
  } else {
    fetch_mem_nums_[index] *= 1.5;
  }
}
} // namespace lmem