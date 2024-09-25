  
#include <filesystem>
#include <string>
#include <vector>

#include <decl.hpp>

namespace App {
  std::filesystem::path GetBinaryPath();
  std::string ReadFile(const std::string& filename);
  std::string ReadShaderCode(std::filesystem::path binary_dir, const std::string& shader_name);
}

