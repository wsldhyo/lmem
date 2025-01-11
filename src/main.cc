
#include "concurrent_allocate.hpp"
int main(int argc, char *argv[]) {
  auto ptr = lmem::concurrent_allocate(15);
  return 0;
}