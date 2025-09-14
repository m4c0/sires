#pragma leco test

import jute;
import hai;
import print;
import sires;

int main() {
  putan("File at:", sires::real_path_name("poc.txt"));

  sires::read("poc.txt", nullptr, [](void *, hai::cstr & data) {
    putfn("Got %d bytes [%s]", data.size(), data.begin());
  });

  sires::read("no-file", nullptr, [](void *, hai::cstr & data) {
    errln("Unreachable");
  });
}
