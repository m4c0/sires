module;
#include <libgen.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

module sires;
import traits;
import yoyo;

namespace {
  auto real_path_name(jute::view name) noexcept {
    using req = mno::req<hai::cstr>;

    hai::cstr buf { 10240 };
    auto res = readlink("/proc/self/exe", buf.data(), buf.size());
    if (res == -1) {
      return req::failed("Failed to find executable name");
    }
    buf.data()[res] = 0;

    const auto dir = dirname(buf.data());
    if (dir != buf.data()) {
      strcpy(buf.data(), dir);
    }

    return req { traits::move(buf) };
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
    struct stat s {};
    stat(name.data(), &s);
    auto mtime = s.st_mtim;
    return static_cast<traits::ints::uint64_t>(mtime.tv_sec) * 1000000000ul
         + static_cast<traits::ints::uint64_t>(mtime.tv_nsec);
  });
}
