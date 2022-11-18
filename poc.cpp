import sires;

#include <iostream>

extern "C" std::streambuf * poc_open() {
  return sires::open("poc", "txt");
}

extern "C" int poc_peek(std::streambuf * ptr) {
  std::unique_ptr<std::streambuf> buf { ptr };
  if (!buf) {
    std::cout << "Failed to read test file\n";
    return 1;
  }

  std::istream in { buf.get() };
  std::string line;
  in >> line;
  std::cout << "Got this from resource: " << line << std::endl;
  return 0;
}

int main() {
  return poc_peek(poc_open());
}
