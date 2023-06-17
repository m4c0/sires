module;
extern "C" void __attribute__((import_name("sires_open"))) sires_open(const char * name, unsigned len, int * ref);
extern "C" int __attribute__((import_name("sires_fsize"))) sires_fsize(int ref);
extern "C" void __attribute__((import_name("sires_read"))) sires_read(int ref, int offs, void * ptr, unsigned len);

module sires;
import hai;
import jute;
import yoyo;

namespace sires {
  class wasm_streambuf : public yoyo::reader {
    char m_buf[1024];
    int m_ref = -1;
    unsigned m_pos = 0;

  public:
    wasm_streambuf(jute::view fname) {
      sires_open(fname.data(), fname.size(), &m_ref);
    }
    [[nodiscard]] req<bool> eof() const noexcept override {
      return req<bool> { sires_fsize(m_ref) == m_pos };
    }
    [[nodiscard]] req<void> seekg(int pos, yoyo::seek_mode mode) noexcept override {
      auto npos = 0;
      auto end = sires_fsize(m_ref);

      switch (mode) {
      case yoyo::seek_mode::set:
        npos = pos;
        break;
      case yoyo::seek_mode::current:
        npos = pos + m_pos;
        break;
      case yoyo::seek_mode::end:
        npos = end + pos;
        break;
      }

      if (npos >= end) return req<void>::failed("buffer overflow");
      if (npos < 0) return req<void>::failed("buffer underflow");
      return {};
    }
    [[nodiscard]] req<unsigned> tellg() const noexcept override {
      return req<unsigned> { m_pos };
    }
    [[nodiscard]] req<unsigned> read_up_to(void * buffer, unsigned len) noexcept override {
      auto npos = m_pos + len;
      auto fsize = sires_fsize(m_ref);
      unsigned l = npos >= fsize ? fsize - m_pos : len;

      sires_read(m_ref, m_pos, buffer, l);

      m_pos = npos >= fsize ? fsize : npos;
      return req<unsigned> { l };
    }
    [[nodiscard]] req<void> read(void * buffer, unsigned len) noexcept override {
      auto npos = m_pos + len;
      if (npos >= sires_fsize(m_ref)) return req<void>::failed("buffer overflow");

      sires_read(m_ref, m_pos, buffer, len);

      m_pos = npos;
      return {};
    }
  };
}

mno::req<hai::uptr<yoyo::reader>> sires::open(jute::view name) noexcept {
  return mno::req { hai::uptr<yoyo::reader> { new wasm_streambuf { name } } };
}
