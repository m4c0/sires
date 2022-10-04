#include "sires.hpp"

#include <fstream>
#include <string>

std::streambuf * sires::open(const char * name, const char * ext) {
  auto fname = std::string { name } + "." + ext;
  auto * res = new std::filebuf();
  res->open(fname, std::ios::binary | std::ios::in);
  return res;
}
