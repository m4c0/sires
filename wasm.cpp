module sires;
import yoyo;

mno::req<hai::cstr> sires::real_path_name(jute::view name) noexcept { return mno::req { name.cstr() }; }
