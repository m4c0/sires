module;
#include <streambuf>

extern "C" std::streambuf * sires_open(const char * name, const char * ext);
module sires;

std::streambuf * sires::open(const char * name, const char * ext) {
  return sires_open(name, ext);
}
