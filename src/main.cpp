
#include <cstdlib>
#include <exception>
#include <iostream>

#include <hello-triangle.hpp>
#include <except.hpp>


void f() {
  throw App::Except::Missing_Item{__PRETTY_FUNCTION__};
}

int main() {

  App::Run();
  
  return EXIT_SUCCESS;
}
