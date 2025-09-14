module;
export module sires;
import hai;
import jojo;
import jute;

namespace sires {
  export hai::cstr real_path_name(jute::view name);

  export void read(jute::view name, void * ptr, auto fn) {
    ::jojo::read(real_path_name(name), ptr, fn);
  }
}

#if LECO_TARGET_ANDROID
#pragma leco add_impl android
#pragma leco add_system_library android
#elif LECO_TARGET_APPLE
#pragma leco add_impl apple
#pragma leco add_framework CoreFoundation
#elif LECO_TARGET_WINDOWS
#pragma leco add_impl windows
#elif LECO_TARGET_WASM
#pragma leco add_impl wasm
#elif LECO_TARGET_LINUX
#pragma leco add_impl linux
#endif
