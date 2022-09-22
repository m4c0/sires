#include "sires.hpp"

#include <fstream>

#import <Foundation/Foundation.h>

std::streambuf * sires::open(const char * name, const char * ext) {
  NSString * name_str = [NSString stringWithUTF8String:name];
  NSString * ext_str = [NSString stringWithUTF8String:ext];
  NSString * path = [[NSBundle mainBundle] pathForResource:name_str ofType:ext_str];

  auto * res = new std::filebuf();
  if (!res->open([path UTF8String], std::ios::binary | std::ios::in)) return nullptr;
  return res;
}

