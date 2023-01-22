module sires;

extern "C" unsigned GetModuleFileNameA(void * handle, char * fname, unsigned size);

mno::req<hai::uptr<yoyo::reader>> sires::open(jute::view name) noexcept {
  // Assume current path as default if GetModuleFileName fails
  jute::view path { "" };

  char exepath[1024];
  auto len = GetModuleFileNameA(0, exepath, sizeof(exepath));
  if (len != 0) {
    path = jute::view { exepath, len };
  }

  const auto & [dir, file] = path.rsplit('\\');

  auto p = (path + "\\"_s + name).cstr();

  return mno::req { hai::uptr<yoyo::reader> { new yoyo::file_reader { p.data() } } };
}
