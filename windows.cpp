#pragma leco add_library user32
module;
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

module sires;
import traits;

hai::cstr sires::real_path_name(jute::view name) {
  // Check if current app has a HWND. If it does not have a window, it most
  // likely means a terminal app - so we are safer to read from current dir
  bool has_hwnd = false;
  EnumWindows([](HWND hwnd, LPARAM lparam) {
    if (GetCurrentProcessId() != GetWindowThreadProcessId(hwnd, NULL)) return TRUE;
    *(bool *)lparam = true;
    return FALSE;
  }, (LPARAM)&has_hwnd);
  if (!has_hwnd) return name.cstr();

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

  return p;
}
