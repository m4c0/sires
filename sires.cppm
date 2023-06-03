export module sires;
export import hai;
export import missingno;
export import jute;
export import traits;
export import yoyo;

namespace sires {
  export mno::req<hai::uptr<yoyo::reader>> open(jute::view name) noexcept;

  export mno::req<hai::holder<char[]>> slurp(jute::view name) {
    return sires::open(name).fmap([](auto & rdr) {
      return rdr->size().fmap([&](auto sz) {
        auto buf = hai::holder<char[]>::make(sz);
        return rdr->read(*buf, sz).map([&] {
          return traits::move(buf);
        });
      });
    });
  }
}
