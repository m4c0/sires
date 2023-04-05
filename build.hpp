#pragma once
#include "../hai/build.hpp"
#include "../jute/build.hpp"
#include "../missingno/build.hpp"
#include "../yoyo/build.hpp"
#include "ecow.hpp"

auto sires() {
  using namespace ecow;

  auto yo = unit::create<per_feat<seq>>("yo");
  yo->for_feature(posix).add_ref(yoyo_libc());
  yo->for_feature(android_ndk).add_ref(yoyo());
  yo->for_feature(webassembly).add_ref(yoyo());

  auto m = unit::create<per_feat<mod>>("sires");
  m->add_wsdep("hai", hai());
  m->add_wsdep("jute", jute());
  m->add_wsdep("missingno", missingno());
  m->add_wsdep("yoyo", yo);
  m->add_feat<inline_js>(
      "sires_open",
      R"((nm_ptr, len, ref_ptr) => {
  const name = new TextDecoder().decode(new Uint8Array(ecow_buffer, nm_ptr, len));
  ecow_globals.sires = ecow_globals.sires || [];
  const ref = ecow_globals.sires.length;
  new Uint32Array(ecow_buffer, ref_ptr, 1)[0] = ref;
  ecow_globals.sires.push(null);
  fetch(name)
    .then(response => response.arrayBuffer())
    .then(bytes => new Uint8Array(bytes))
    .then(array => ecow_globals.sires[ref] = array)
    .then(_ => window.dispatchEvent(new CustomEvent('sires_opened', { detail: ref })));
})");
  m->add_feat<inline_js>("sires_fsize", R"((ref) => {
  return ecow_globals.sires[ref].length;
})");
  m->add_feat<inline_js>("sires_read", R"((ref, offs, ptr, len) => {
  const buf = ecow_globals.sires[ref].subarray(offs, len);
  new Uint8Array(ecow_buffer, ptr, len).set(buf);
})");

  m->for_feature(android_ndk).add_impl("android");
  m->for_feature(android_ndk).add_system_library("android");

  m->for_feature(objective_c).add_impl("apple");
  m->for_feature(objective_c).add_unit<objc>("apple");
  m->for_feature(webassembly).add_impl("wasm");
  m->for_feature(windows_api).add_impl("windows");

  return m;
}
