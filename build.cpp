#include "../hai/build.hpp"
#include "../jute/build.hpp"
#include "../missingno/build.hpp"
#include "../yoyo/build.hpp"
#include "ecow.hpp"

using namespace ecow;

int main(int argc, char ** argv) {
  seq all { "all" };

  auto m = all.add_unit<per_feat<mod>>("sires");
  m->add_wsdep("hai", hai());
  m->add_wsdep("jute", jute());
  m->add_wsdep("missingno", missingno());
  m->add_wsdep("yoyo", yoyo());
  m->add_feat<js>()->set(
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
    .then(array => ecow_globals.sires[ref] = array);
})");
  m->add_feat<js>()->set("sires_is_ready", R"((ref) => {
  return !!ecow_globals.sires[ref];
})");
  m->add_feat<js>()->set("sires_read", R"((ref, offs, ptr, len) => {
  const buf = ecow_globals.sires[ref].subarray(offs, len);
  new Uint8Array(ecow_buffer, ptr, len).set(buf);
  return buf.length;
}
)");

  m->for_feature(objective_c).add_impl("apple");
  m->for_feature(webassembly).add_impl("wasm");
  m->for_feature(windows_api).add_impl("windows");

  auto poc = all.add_unit<app>("sires-poc");
  poc->add_feat<js>()->set("main", "");
  poc->add_ref(m);
  poc->add_unit<>("poc");
  poc->add_resource("poc.txt");

  return run_main(all, argc, argv);
}
