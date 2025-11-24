#pragma leco test

import jute;
import hai;
import jojo;
import print;
import sires;

int main() {
  putan("File at:", sires::real_path_name("poc.txt"));

  static auto id = 0;
  jojo::on_error([](void * ptr, jute::view msg) {
    if (&id != ptr) die("Got error: ", msg);
    putan("Got expected error:", msg);
  });

  sires::read("poc.txt", nullptr, [](void *, hai::cstr & data) {
    putfn("Got %d bytes [%s]", data.size(), data.begin());
  });

  sires::read("no-file", &id, [](void *, hai::cstr & data) {
    die("Unreachable");
  });
}
