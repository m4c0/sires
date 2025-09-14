module;
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

module sires;
import traits;

hai::cstr sires::real_path_name(jute::view name) {
  char buf[128];
  // If running as a terminal app
  // TODO: validate this on a real Windows machine
  if (GetEnvironmentVariable("TERM", buf, sizeof(buf)) > 0) {
    return name.cstr();
  }

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
  // TODO: communicate this via on_error
  if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY)) return {};

  return traits::move(p);
}
