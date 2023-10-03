#pragma leco app
#pragma leco add_resource "poc.txt"

import silog;
import sires;

#ifndef __wasm__
#define EXPORT(x)
#else
#define EXPORT(x) __attribute__((export_name(x)))
#endif

extern "C" yoyo::reader * EXPORT("poc_open") poc_open() {
  using namespace jute::literals;

  return sires::open("poc.txt"_s)
      .map([](auto & u) {
        return u.release();
      })
      .unwrap(nullptr);
}
extern "C" int EXPORT("poc_read") poc_read(yoyo::reader * rdr) {
  char buf[5] {};
  return rdr->read(buf, 4)
      .map([&] {
        silog::log(silog::info, "Got: (%s)", buf);
        return 0;
      })
      .unwrap(1);
}

int main() {
  using namespace jute::literals;

  char buf[5] {};
  return sires::open("poc.txt"_s)
      .fmap([&](auto && rdr) {
        while (!rdr->ready()) {
          // sleep, update ui, etc
        }
        return rdr->read(buf, 4);
      })
      .map([&] {
        silog::log(silog::info, "Got: (%s)", buf);
      })
      .fmap([] {
        return sires::stat("poc.txt");
      })
      .map([](auto ts) {
        silog::log(silog::info, "Mod. timestamp: %lu", ts);
        return 0;
      })
      .take([](auto err) {
        silog::log(silog::error, "Error: %s", err);
      });
}
