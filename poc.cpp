import silog;
import sires;

extern "C" yoyo::reader * poc_open() {
  using namespace jute::literals;

  return sires::open("poc.txt"_s)
      .map([](auto & u) {
        return u.release();
      })
      .unwrap(nullptr);
}
extern "C" int poc_read(yoyo::reader * rdr) {
  char buf[5] {};
  return rdr->read(buf, 4)
      .map([&] {
        silog::log(silog::info, buf);
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
        silog::log(silog::info, buf);
        return 0;
      })
      .unwrap(1);
}
