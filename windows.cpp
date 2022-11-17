module;

#include <filesystem>
#include <fstream>
#include <tchar.h>
#include <windows.h>

module sires;

inline auto exe_path() noexcept {
  TCHAR exepath[MAX_PATH + 1];

  if (GetModuleFileName(0, exepath, MAX_PATH + 1) == 0) {
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
