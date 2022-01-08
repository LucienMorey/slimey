//
// Created by xflajs00 on 22.10.2021.
//

#include "utils/files.h"
#include <fstream>
namespace physarum{
std::filesystem::path getExeFolder() {
  const auto currentPath = std::filesystem::current_path();
  return absolute(currentPath);
}
std::optional<std::string> readFile(const std::filesystem::path &path) {
  std::ifstream ifstream{path};
  if (!ifstream.is_open()) { return std::nullopt; }
  return std::string{std::istreambuf_iterator<char>{ifstream}, std::istreambuf_iterator<char>{}};
}
}