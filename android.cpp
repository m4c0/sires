module;
#include <bits/seek_constants.h>

struct AAsset;
struct AAssetManager;

extern "C" void AAsset_close(AAsset * asset);
extern "C" unsigned AAsset_getLength(AAsset * asset);
extern "C" unsigned AAsset_getRemainingLength(AAsset * asset);
extern "C" AAsset * AAssetManager_open(AAssetManager * mgr, const char * filename, int mode);
extern "C" int AAsset_read(AAsset * asset, void * buf, unsigned count);
extern "C" unsigned AAsset_seek(AAsset * asset, unsigned offset, int whence);

module sires;

namespace sires {
  static AAssetManager * mgr;

  struct closer {
    void operator()(AAsset * a) {
      AAsset_close(a);
    }
  };
  class droid_streambuf : public yoyo::reader {
    hai::holder<AAsset, closer> m_asset;

    [[nodiscard]] static constexpr auto whence_of(yoyo::seek_mode mode) noexcept {
      switch (mode) {
      case yoyo::seek_mode::set:
        return SEEK_SET;
      case yoyo::seek_mode::current:
        return SEEK_CUR;
      case yoyo::seek_mode::end:
        return SEEK_END;
      }
    }

  public:
    droid_streambuf(jute::view fname) : m_asset { AAssetManager_open(mgr, fname.data(), 1) } {
    }
    [[nodiscard]] req<bool> eof() const noexcept override {
      return req<bool> { AAsset_getRemainingLength(*m_asset) > 0 };
    }
    [[nodiscard]] req<void> seekg(int pos, yoyo::seek_mode mode) noexcept override {
      if (AAsset_seek(*m_asset, pos, whence_of(mode)) == -1) return req<void>::failed("failed to seek");
      return {};
    }
    [[nodiscard]] req<unsigned> tellg() const noexcept override {
      return req<unsigned> { AAsset_getLength(*m_asset) - AAsset_getRemainingLength(*m_asset) };
    }
    [[nodiscard]] req<void> read(void * buffer, unsigned len) noexcept override {
      if (AAsset_read(*m_asset, buffer, len) != len) return req<void>::failed("failed to read");
      return {};
    }
  };
}

mno::req<hai::uptr<yoyo::reader>> sires::open(jute::view name) noexcept {
  return mno::req { hai::uptr<yoyo::reader> { new droid_streambuf { name } } };
}
