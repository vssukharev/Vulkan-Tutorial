
#include <hello-triangle.hpp>
#include <helpers/files.hpp>
#include <debug.hpp>

void App::Init(Meta& meta)
{
  meta.binary_dir = GetBinaryPath().parent_path();
  Dbg::PrintFunctionInfo(__PRETTY_FUNCTION__, "Detected binary directory = ", meta.binary_dir);
}

