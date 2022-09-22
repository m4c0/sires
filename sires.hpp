#pragma once
#include <streambuf>

namespace sires {
  std::streambuf * open(const char * name, const char * ext);
}
