#pragma once
#include "../yoyo/build.hpp"
#include "ecow.hpp"

auto sires() {
  using namespace ecow;

  auto m = unit::create<box>("sires");
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
  return m;
}
