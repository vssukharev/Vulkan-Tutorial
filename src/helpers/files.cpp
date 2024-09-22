
#include <bits/types/error_t.h>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <cstdlib>
#include <iterator>
#include <limits.h>

#include <implementation.hpp>
#include <except.hpp>
#include <config.hpp>
#include <debug.hpp>
#include <helpers/files.hpp>


/// 
std::vector<char> App::Impl::ReadFile(const std::string& filename)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary );

  if (!file.is_open())
    throw Except::File_Open_Failure{filename.c_str()};

  size_t file_size = file.tellg();
  std::vector<char> buffer(file_size);

  Dbg::PrintFunctionInfo(__FUNCTION__, "File size = ", file_size, "; Buffer size = ", buffer.size());

  file.seekg(0);
  file.read(buffer.data(), file_size);
  file.close();

  return buffer;
}

/// Assumes that you are in root directory of project
std::vector<char> App::Impl::ReadShaderCode(std::filesystem::path binary_dir, const std::string& shader_name)
{
  binary_dir /= SHADERS_DIR;
  binary_dir /= shader_name;

  Dbg::PrintFunctionInfo(__FUNCTION__, "Shader's full path = ", binary_dir);
 
  std::vector<char> shader_code = Impl::ReadFile(binary_dir);
  return shader_code;
}

///
std::filesystem::path App::Impl::GetBinaryPath()
{
  std::filesystem::path res;
  res = std::filesystem::read_symlink("/proc/self/exe");
  return res;
}


