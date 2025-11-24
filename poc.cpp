#pragma leco app
#pragma leco add_resource "poc.txt"

import jute;
import hai;
import jojo;
import silog;
import sires;

int main() {
  silog::log(silog::debug, "File at: %s", sires::real_path_name("poc.txt").begin());

  static auto id = 0;
  jojo::on_error([](void * ptr, jute::view msg) {
    if (&id != ptr) silog::log(silog::error, "Got error: %s", msg.cstr().begin());
  });

  sires::read("poc.txt", nullptr, [](void *, hai::cstr & data) {
    silog::log(silog::debug, "Got %d bytes [%s]", data.size(), data.begin());
  });

  sires::read("no-file", nullptr, [](void *, hai::cstr & data) {
    silog::die("Unreachable");
  });

#ifndef LECO_TARGET_WASM
  silog::log(silog::debug, "Reading again: %s", sires::slurp("poc.txt").begin());
#endif
}
