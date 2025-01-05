#include "concurrent_allocate.hpp"
#include "thread_cache.hpp"
namespace lmem {

void *concurrent_allocate(std::size_t size) {
  if (g_thread_cache == nullptr) {
    g_thread_cache = new ThreadCache;
  }
  return g_thread_cache->allocate(size);
}

void concurrent_deallocate(void *obj, std::size_t aligned_size) {
  g_thread_cache->deallocate(obj, aligned_size);
}
} // namespace lmem