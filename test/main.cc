
#include "concurrent_allocate.hpp"
int main(int argc, char *argv[]) {
  auto ptr1 = lmem::concurrent_allocate(15);
  auto ptr2 = lmem::concurrent_allocate(3);
  auto ptr3 = lmem::concurrent_allocate(82);
  auto ptr4 = lmem::concurrent_allocate(16);
  auto ptr5 = lmem::concurrent_allocate(278);

  lmem::concurrent_deallocate(ptr1, 15);
  lmem::concurrent_deallocate(ptr2, 3);
  lmem::concurrent_deallocate(ptr3, 82);
  lmem::concurrent_deallocate(ptr4, 16);
  lmem::concurrent_deallocate(ptr5, 278);
  return 0;
}