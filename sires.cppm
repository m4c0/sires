module;
#include <streambuf>

export module sires;

export namespace sires {
  std::streambuf * open(const char * name, const char * ext);
}
