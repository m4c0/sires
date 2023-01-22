module;
// Until I understand why this is needed only for wasm
#include <new>

extern "C" void sires_open(const char * name, unsigned len, int * ref);
extern "C" bool sires_is_ready(int ref);
extern "C" int sires_read(int ref, int offs, void * ptr, unsigned len);

module sires;
import hai;
import jute;
import yoyo;

namespace sires {
  class wasm_streambuf : public yoyo::reader {
    char m_buf[1024];
    int m_ref = -1;

  public:
    wasm_streambuf(jute::view fname) {
      sires_open(fname.data(), fname.size(), &m_ref);
    }
    [[nodiscard]] bool ready() const noexcept override {
      return sires_is_ready(m_ref);
    }
    [[nodiscard]] req<bool> eof() const noexcept override {
      return {};
    }
    [[nodiscard]] req<void> seekg(int pos, yoyo::seek_mode mode) noexcept override {
      return {};
    }
    [[nodiscard]] req<unsigned> tellg() const noexcept override {
      return {};
    }
    [[nodiscard]] req<void> read(void * buffer, unsigned len) noexcept override {
      return {};
    }
  };
}

mno::req<hai::uptr<yoyo::reader>> sires::open(jute::view name) noexcept {
  return mno::req { hai::uptr<yoyo::reader> { new wasm_streambuf { name } } };
}
