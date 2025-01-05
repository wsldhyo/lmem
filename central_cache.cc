#include "central_cache.hpp"

namespace lmem {
CentralCache *CentralCache::get_instance() {
  if (instance_ == nullptr) {
    instance_ = new CentralCache();
  }
  return instance_;
}
} // namespace lmem