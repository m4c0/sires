module;
#define MTIME_IMPLEMENTATION
#include "../mtime/mtime.h"

export module sires;
export import hai;
export import missingno;
export import jute;
export import traits;
export import yoyo;

namespace sires {
  mno::req<hai::cstr> real_path_name(jute::view name) noexcept;

  export mno::req<hai::uptr<yoyo::reader>> open(jute::view name) noexcept {
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

  export mno::req<hai::array<char>> slurp(jute::view name) {
    return sires::open(name).fmap([](auto & rdr) {
      return rdr->size().fmap([&](auto sz) {
        hai::array<char> buf { sz };
        return rdr->read(buf.begin(), sz).map([&] {
          return traits::move(buf);
        });
      });
    });
  }

  export mno::req<traits::ints::uint64_t> stat(jute::view name) noexcept {
    constexpr const auto mtime_succeeded = [](auto t) {
      return t != 0;
    };
    return real_path_name(name)
        .map([](auto & name) -> traits::ints::uint64_t {
          return mtime_of(name.data());
        })
        .assert(mtime_succeeded, "Failed to open resource");
  }
}

#if __ANDROID__
#pragma leco add_impl android
#pragma leco add_system_library android
#elif __APPLE__
#pragma leco add_impl apple
#pragma leco add_framework CoreFoundation
#elif _WIN32
#pragma leco add_impl windows
#elif WASM
#pragma leco add_impl wasm
#elif __linux__
#pragma leco add_impl linux
#endif
