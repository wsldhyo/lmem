#ifndef CONCURRENT_ALLOCATE_HPP
#define CONCURRENT_ALLOCATE_HPP
#include <cstddef>
namespace lmem {
void *concurrent_allocate(std::size_t size);

void concurrent_deallocate(void *obj, std::size_t aligned_size);
} // namespace lmem

#endif