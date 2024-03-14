module;
#define WIN32_LEAN_AND_MEAN
#include <sys/stat.h>
#include <windows.h>

module sires;
import traits;
import yoyo;

namespace {
  using req = mno::req<hai::uptr<yoyo::reader>>;

  auto real_path_name(jute::view name) noexcept {
    using namespace jute::literals;
    using req = mno::req<hai::cstr>;

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

    return req { traits::move(p) };
  }
}
mno::req<hai::uptr<yoyo::reader>> sires::open(jute::view name) noexcept {
  return real_path_name(name)
      .fmap([](auto & path) {
        return yoyo::file_reader::open(path.data());
      })
      .map([](auto && fr) {
        return new yoyo::file_reader { traits::move(fr) };
      })
      .map([](auto * r) {
        return hai::uptr<yoyo::reader> { r };
      });
}
mno::req<traits::ints::uint64_t> sires::stat(jute::view name) noexcept {
  return real_path_name(name).map([](auto & name) -> traits::ints::uint64_t {
    struct __stat64 s {};
    _stat64(name.data(), &s);
    return s.st_mtime;
  });
}
