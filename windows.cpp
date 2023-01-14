module;

#include <filesystem>
#include <fstream>

module sires;

extern "C" unsigned GetModuleFileNameA(void * handle, char * fname, unsigned size);

inline auto exe_path() noexcept {
  char exepath[1024];

  if (GetModuleFileNameA(0, exepath, sizeof(exepath)) == 0) {
    std::terminate();
  }

  return std::filesystem::path { exepath };
}

std::streambuf * sires::open(const char * name, const char * ext) {
  const auto path = exe_path().replace_filename(name).replace_extension(ext);
  auto * res = new std::filebuf();
  if (!res->open(path, std::ios::binary | std::ios::in)) return nullptr;
  return res;
}
