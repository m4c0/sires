module;
#define WIN32_LEAN_AND_MEAN
#include <sys/stat.h>
#include <windows.h>

module sires;
import traits;
import yoyo;

hai::cstr sires::real_path_name(jute::view name) {
  using namespace jute::literals;

  // Assume current path as default if GetModuleFileName fails
  auto path = ""_s;

  char exepath[1024];
  auto len = GetModuleFileNameA(0, exepath, sizeof(exepath));
  if (len != 0) {
    path = jute::view { exepath, len };
  }

  const auto & [dir, file] = path.rsplit('\\');

  auto p = (dir + "\\"_s + name).cstr();
  DWORD attr = GetFileAttributes(p.data());
  if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY)) return {};

  return traits::move(p);
}
