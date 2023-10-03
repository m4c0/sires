export module sires;
export import hai;
export import missingno;
export import jute;
export import traits;
export import yoyo;

namespace sires {
  export mno::req<hai::uptr<yoyo::reader>> open(jute::view name) noexcept;

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

  export mno::req<traits::ints::uint64_t> stat(jute::view name) noexcept;
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
#endif
