module;
#include <fstream>

module sires;

std::streambuf * sires::open(const char * name, const char * ext) {
  auto * res = new std::filebuf();
  res->open(std::string { name } + "." + ext, std::ios::binary | std::ios::in);
  return res;
}
