module;
#define WIN32_LEAN_AND_MEAN
#include <sys/stat.h>
#include <windows.h>

module sires;
import traits;
import yoyo;

namespace {
  using req = mno::req<hai::uptr<yoyo::reader>>;
}
req sires::open(jute::view name) noexcept {
  using namespace jute::literals;

  // Assume current path as default if GetModuleFileName fails
  auto path = ""_s;

  char exepath[1024];
  auto len = GetModuleFileNameA(0, exepath, sizeof(exepath));
  if (len != 0) {
    path = jute::view { exepath, len };
  }

  const auto & [dir, file] = path.rsplit('\\');

  auto p = (dir + "\\"_s + name).cstr();

  DWORD attr = GetFileAttributes(p.data());
  if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY)) return req::failed("Resource not found");

  return mno::req { hai::uptr<yoyo::reader> { new yoyo::file_reader { p.data() } } };
}
traits::ints::uint64_t sires::stat(const char * name) noexcept {
  struct __stat64 s {};
  _stat64(name, &s);
  return s.st_mtime;
}
