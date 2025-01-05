#include <algorithm>
#include <cassert>
#include <cstddef>

#include "global_var.hpp"
#include "size_class.hpp"
namespace lmem {
std::size_t SizeClass::round_up(std::size_t size) {
  if (size <= 128) {
    return round_up_(size, 8);
  } else if (size <= 1024) {
    return round_up_(size, 16);
  } else if (size <= 8 * 1024) {
    return round_up_(size, 128);
  } else if (size <= 64 * 1024) {
    return round_up_(size, 1024);
  } else if (size <= 256 * 1024) {
    return round_up_(size, 8 * 1024);
  } else {
    assert(false);
    return -1;
  }
}

std::size_t SizeClass::round_up_(std::size_t size, std::size_t align_val) {
  std::size_t res = size;
  if (size % align_val) {
    // 有余数，整除后+1, 再乘align_val，可以得到比size稍大，且是align_val的倍数
    res = (size / align_val + 1) * align_val;
  }
  return res;
}

std::size_t SizeClass::get_hash_index(std::size_t size)

{
  assert(size <= MAX_TC_BLOCK_SIZE);
  static int range_list_nums[4] = {16, 56, 56, 56};
  if (size <= 128) {
    return get_hash_index_(size, 3);
  } else if (size <= 1024) {
    return get_hash_index_(size - 128, 4) + range_list_nums[0];
  } else if (size <= 8 * 1024) {
    return get_hash_index_(size - 1024, 7) + range_list_nums[0] +
           range_list_nums[1];
  } else if (size <= 64 * 1024) {
    return get_hash_index_(size - 8 * 1024, 10) + range_list_nums[0] +
           range_list_nums[1] + range_list_nums[2];
  } else if (size <= 256 * 1024) {
    return get_hash_index_(size - 64 * 1024, 13);
    +range_list_nums[0] + range_list_nums[1] + range_list_nums[2] +
        range_list_nums[3];
  } else {
    assert(false);
  }
  return -1;
}

std::size_t SizeClass::get_hash_index_(std::size_t size,
                                       std::size_t align_shift) {
  // 1 << align_shift是对齐值
  return ((size + (1 << align_shift) - 1) >> align_shift) - 1;
}

std::size_t SizeClass::cacl_apply_mem_nums(std::size_t size,
                                           std::size_t max_num) {
  assert(size > 0);
  std::size_t num = 2;
  if (MAX_TC_BLOCK_SIZE / size > 512) {
    num = 512;
  }
  return std::min(num, max_num);
}

} // namespace lmem