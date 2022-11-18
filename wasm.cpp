module;
#include <fstream>

extern "C" void sires_open(const char * name, unsigned len, int * ref);
extern "C" int sires_read(int ref, int offs, void * ptr, unsigned len);

module sires;

namespace sires {
  class wasm_streambuf : public std::streambuf {
    char m_buf[1024];
    int m_ref;
    int m_sofar;

  protected:
    int underflow() override {
      const auto nr = sires_read(m_ref, m_sofar, m_buf, sizeof(m_buf));
      if (nr == 0) return traits_type::eof();
      m_sofar += nr;
      setg(m_buf, m_buf, m_buf + sizeof(m_buf));
      return nr;
    }

  public:
    explicit wasm_streambuf(const std::string & name) {
      sires_open(name.c_str(), name.size(), &m_ref);
    }
  };
}

std::streambuf * sires::open(const char * name, const char * ext) {
  return new wasm_streambuf { std::string { name } + "." + ext };
}
