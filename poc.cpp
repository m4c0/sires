#include <stdio.h>

import sires;

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
        printf("got: [%s]\n", buf);
        return 0;
      })
      .unwrap(1);
}
