#pragma leco app
#pragma leco add_resource "poc.txt"

import jute;
import hai;
import silog;
import sires;

int main() {
  silog::log(silog::debug, "File at: %s", sires::real_path_name("poc.txt").begin());

  sires::read("poc.txt", nullptr, [](void *, hai::cstr & data) {
    silog::log(silog::debug, "Got %d bytes [%s]", data.size(), data.begin());
  });

  sires::read("no-file", nullptr, [](void *, hai::cstr & data) {
    silog::log(silog::error, "Unreachable");
    throw 0;
  });
}
