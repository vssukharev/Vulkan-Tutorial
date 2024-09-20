

#include "hello-triangle.hpp"
#include <implementation.hpp>
#include <iostream>


// --- Data_Class ---
App::Data_Class::Data_Class() : pdata{new Data{}} {}
App::Data_Class::~Data_Class() { delete pdata; }
App::Data& App::Data_Class::get() { return *pdata; }
App::Data_Class::operator Data&() { return *pdata; }
// ------------------


/// Fully runs application
void App::Run() 
{
  Data data {};

  Init(data); 
  MainLoop(data);
  Cleanup(data);
}


/// Initializes application
void App::Init(Data& data)
{
  Init(data.vulkan);
}


/// Loops application
void App::MainLoop(Data& data) 
{
  while ( !glfwWindowShouldClose(data.vulkan.window) ) 
  {
    glfwPollEvents();
    DrawFrame(data.vulkan);
  }

  vkDeviceWaitIdle(data.vulkan.device);
}


/// Collects garbafe left after application
void App::Cleanup(Data& data)
{
  Cleanup(data.vulkan);
}

