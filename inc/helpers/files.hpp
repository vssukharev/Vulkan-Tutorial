  
#include <filesystem>
#include <string>
#include <vector>

namespace App::Impl {
  std::filesystem::path GetBinaryPath();
  std::vector<char> ReadFile(const std::string& filename);
  std::vector<char> ReadShaderCode(std::filesystem::path binary_dir, const std::string& shader_name);
}

