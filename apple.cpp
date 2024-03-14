module;
#include <CoreFoundation/CoreFoundation.h>
#include <sys/stat.h>

module sires;
import traits;
import yoyo;

namespace {
  struct cfdel {
    void operator()(const void * ptr) {
      if (ptr) CFRelease(ptr);
    }
  };
  template<typename T>
  using cfptr = hai::holder<traits::remove_ptr_t<T>, cfdel>;

  auto real_path_name(jute::view name) noexcept {
    using req = mno::req<hai::cstr>;

    cfptr<CFStringRef> nsname { CFStringCreateWithBytesNoCopy(
        nullptr,
        (const UInt8 *)name.data(),
        name.size(),
        kCFStringEncodingUTF8,
        false,
        kCFAllocatorNull) };
    auto bundle = CFBundleGetMainBundle();
    cfptr<CFURLRef> url { CFBundleCopyResourceURL(bundle, *nsname, nullptr, nullptr) };
    if (!*url) return req::failed("Could not find resource file");

    cfptr<CFStringRef> path { CFURLCopyFileSystemPath(*url, kCFURLPOSIXPathStyle) };
    if (!*path) return req::failed("Could not open resource file");

    hai::cstr p { static_cast<unsigned>(CFStringGetLength(*path)) };
    CFStringGetCString(*path, p.data(), p.size() + 1, kCFStringEncodingUTF8);

    return req { traits::move(p) };
  }
}
mno::req<hai::uptr<yoyo::reader>> sires::open(jute::view name) noexcept {
  return real_path_name(name)
      .fmap([](auto & path) {
        return yoyo::file_reader::open(path.data());
      })
      .map([](auto && fr) {
        return new yoyo::file_reader { traits::move(fr) };
      })
      .map([](auto * r) {
        return hai::uptr<yoyo::reader> { r };
      });
}
mno::req<traits::ints::uint64_t> sires::stat(jute::view name) noexcept {
  return real_path_name(name).map([](auto & name) -> traits::ints::uint64_t {
    struct stat s {};
    stat(name.data(), &s);
    auto mtime = s.st_mtimespec;
    return static_cast<uint64_t>(mtime.tv_sec) * 1000000000ul + static_cast<uint64_t>(mtime.tv_nsec);
  });
}
