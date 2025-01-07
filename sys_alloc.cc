#include "sys_alloc.hpp"
#include <cstddef>
#include <new>
#define LINUX
namespace lmem {
void* system_allocate(std::size_t page_nums){
 void* ptr = nullptr;
#ifdef WIN32
  ptr = VirtualAlloc(0, page_nums << PAGE_SHIFT, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); 
#elif defined (LINUX)
  ;// TODO
#endif
    if (ptr == nullptr) {
        throw std::bad_alloc();
    }
    return ptr;
}
}