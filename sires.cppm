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
}
