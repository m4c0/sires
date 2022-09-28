#include "sires.hpp"

#include <fstream>

std::streambuf * sires::open(const char * name, const char * ext) {
  auto * res = new std::filebuf();
  res->open(name, std::ios::binary | std::ios::in);
  return res;
}
