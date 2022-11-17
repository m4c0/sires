import sires;

#include <iostream>

int main() {
  std::unique_ptr<std::streambuf> buf { sires::open("poc", "txt") };
  if (!buf) {
    std::cout << "Failed to read test file\n";
    return 1;
  }

  std::cout << "OK\n";
}
