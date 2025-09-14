module;
#include <CoreFoundation/CoreFoundation.h>
#include <sys/stat.h>

extern "C" void NSLog(CFStringRef format, ...);

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
}

hai::cstr sires::real_path_name(jute::view name) {
  cfptr<CFStringRef> nsname { CFStringCreateWithBytesNoCopy(
      nullptr,
      (const UInt8 *)name.data(),
      name.size(),
      kCFStringEncodingUTF8,
      false,
      kCFAllocatorNull) };
  auto bundle = CFBundleGetMainBundle();
  if (!CFBundleGetIdentifier(bundle)) {
    // Non-bundled app. Let's accept local files
    return name.cstr();
  }

  cfptr<CFURLRef> url { CFBundleCopyResourceURL(bundle, *nsname, nullptr, nullptr) };
  if (!*url) return {};

  cfptr<CFStringRef> path { CFURLCopyFileSystemPath(*url, kCFURLPOSIXPathStyle) };
  if (!*path) return {};

  hai::cstr p { static_cast<unsigned>(CFStringGetLength(*path)) };
  CFStringGetCString(*path, p.data(), p.size() + 1, kCFStringEncodingUTF8);

  return traits::move(p);
}
