#pragma once
#include <filesystem>

std::string gencpp(const std::filesystem::path& brofile, bool hppmode = false);
