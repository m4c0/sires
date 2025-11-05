module;
#include <libgen.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

module sires;

hai::cstr sires::real_path_name(jute::view name) {
  char buf[1024];
  auto res = readlink("/proc/self/exe", buf, sizeof(buf));
  if (res == -1) {
    return {};
  }
  buf[res] = 0;

  auto dir = jute::view::unsafe(dirname(buf));
  return (dir + "/" + name).cstr();
}
