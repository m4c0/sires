#include "ecow.hpp"

using namespace ecow;

int main(int argc, char ** argv) {
  seq all { "all" };

  auto m = all.add_unit<per_feat<mod>>("sires");

  auto & m_steve = m->for_feature(objective_c);
  m_steve.add_impl("apple");
  m_steve.add_unit<objc>("apple.mm")->add_framework("Foundation");

  m->for_feature(webassembly).add_impl("wasm");
  m->for_feature(windows_api).add_impl("windows");

  auto poc = all.add_unit<app>("sires-poc");
  poc->add_ref(m);
  poc->add_unit<>("poc");
  // TODO: add poc.txt as a resource

  return run_main(all, argc, argv);
}
