#pragma leco app
#pragma leco add_resource "poc.txt"

import jute;
import hai;
import silog;
import sires;

int main() {
  sires::read("poc.txt", nullptr, [](void *, hai::cstr & data) {
    silog::log(silog::debug, "Got %d bytes [%s]", data.size(), data.begin());
  });
}
