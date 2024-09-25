
#include <bits/types/error_t.h>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <cstdlib>
#include <limits.h>

#include <hello-triangle.hpp>
#include <except.hpp>
#include <config.hpp>
#include <debug.hpp>
#include <helpers/files.hpp>


/// 
std::string App::ReadFile(const std::string& filename)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary );

  if (!file.is_open())
    throw Except::File_Open_Failure{filename.c_str()};

  size_t file_size = file.tellg();
  std::string buffer;
  buffer.resize(file_size);

  Dbg::PrintFunctionInfo(__FUNCTION__, "File size = ", file_size, "; Buffer size = ", buffer.size());

  file.seekg(0);
  file.read(buffer.data(), file_size);
  file.close();

  return buffer;
}

/// Assumes that you are in root directory of project
std::string App::ReadShaderCode(std::filesystem::path binary_dir, const std::string& shader_name)
{
  binary_dir /= SHADERS_DIR;
  binary_dir /= shader_name;

  Dbg::PrintFunctionInfo(__FUNCTION__, "Shader's full path = ", binary_dir);

  std::string shader_code = ReadFile(binary_dir);
  return shader_code;
}

///
std::filesystem::path App::GetBinaryPath()
{
  std::filesystem::path res;
  res = std::filesystem::read_symlink("/proc/self/exe");
  return res;
}


