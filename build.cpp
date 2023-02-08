#include "build.hpp"

using namespace ecow;

int main(int argc, char ** argv) {
  auto m = sires();

  seq all { "all" };
  all.add_ref(m);

  auto poc = all.add_unit<app>("sires-poc");
  poc->add_feat<js>()->set("poc_open", "");
  poc->add_feat<js>()->set("poc_read", "");
  poc->add_ref(m);
  poc->add_unit<>("poc");
  poc->add_resource("poc.txt");

  return run_main(all, argc, argv);
}
