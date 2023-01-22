export module sires;
export import hai;
export import missingno;
export import jute;
export import yoyo;

namespace sires {
  export mno::req<hai::uptr<yoyo::reader>> open(jute::view name) noexcept;
}
