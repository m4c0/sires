#include <wasi/api.h>

extern "C" int __imported_wasi_snapshot_preview1_path_open(
    int fd,
    __wasi_lookupflags_t dirflags,
    const char * path,
    size_t len,
    __wasi_oflags_t oflags,
    __wasi_rights_t fs_rights_base,
    __wasi_rights_t fs_rights_inheriting,
    __wasi_fdflags_t fdflags,
    __wasi_fd_t * retptr0) {
  *retptr0 = fd;
  return __WASI_ERRNO_SUCCESS;
}
