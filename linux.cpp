module;
#include <libgen.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

module sires;
import traits;
import yoyo;

mno::req<hai::cstr> sires::real_path_name(jute::view name) noexcept {
  using req = mno::req<hai::cstr>;

  char buf[1024];
  auto res = readlink("/proc/self/exe", buf, sizeof(buf));
  if (res == -1) {
    return req::failed("Failed to find executable name");
  }
  buf[res] = 0;

  auto dir = jute::view::unsafe(dirname(buf));
  auto p = (dir + "/" + name).cstr();

  return req { traits::move(p) };
}
