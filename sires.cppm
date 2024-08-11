module;
export module sires;
import hai;
import missingno;
import mtime;
import jute;
import traits;
import yoyo;

namespace sires {
  hai::cstr real_path_name(jute::view name);

  export mno::req<yoyo::file_reader> open(jute::view name) noexcept {
    auto rp = real_path_name(name);
    if (rp.size() == 0) return mno::req<yoyo::file_reader>::failed("Could not find resource");
    return yoyo::file_reader::open(real_path_name(name).data());
  }

  export mno::req<hai::array<char>> slurp(jute::view name) {
    return sires::open(name).fmap([](auto & rdr) {
      return rdr.size().fmap([&](unsigned sz) {
        hai::array<char> buf { sz };
        return rdr.read(buf.begin(), sz).map([&] {
          return traits::move(buf);
        });
      });
    });
  }

  export mno::req<traits::ints::uint64_t> stat(jute::view name) noexcept {
    auto rp = real_path_name(name);
    if (rp.size() == 0) return mno::req<traits::ints::uint64_t>::failed("Could not find resource");
    auto t = mtime::of(rp.data());
    return t > 0 ? mno::req{ t } : mno::req<traits::ints::uint64_t>::failed("Could not check mod time of resource");
  }
}

#if LECO_TARGET_ANDROID
#pragma leco add_impl android
#pragma leco add_system_library android
#elif LECO_TARGET_APPLE
#pragma leco add_impl apple
#pragma leco add_framework CoreFoundation
#elif LECO_TARGET_WINDOWS
#pragma leco add_impl windows
#elif LECO_TARGET_WASM
#pragma leco add_impl wasm
#elif LECO_TARGET_LINUX
#pragma leco add_impl linux
#endif
