#pragma leco app
#pragma leco add_resource "poc.txt"

import jute;
import hai;
import silog;
import sires;

int main() {
  sires::jojo("poc.txt", nullptr, [](void *, hai::cstr & data) {
    silog::log(silog::debug, "Got %d bytes [%.*s]", data.size(), data.size(), data.begin());
  });

#ifndef LECO_TARGET_WASM
  using namespace jute::literals;

  char buf[5] {};
  return sires::open("poc.txt"_s)
      .trace("opening file")
      .fmap([&](auto && rdr) {
        while (!rdr.ready()) {
          // sleep, update ui, etc
        }
        return rdr.read(buf, 4).trace("reading file");
      })
      .map([&] {
        silog::log(silog::info, "Got: (%s)", buf);
      })
      .fmap([] {
        return sires::stat("poc.txt").trace("stating file");
      })
      .map([](auto ts) {
        silog::log(silog::info, "Mod. timestamp: %llu", ts);
        return 0;
      })
      .log_error();
#endif
}
