
#pragma once

#include <iostream>

#include <config.hpp>

#define EXCEPT_NAME_RET inline static const char*


// ------ Exception classes ------
namespace App::Except {

  // --- Base class ---
  struct Base_Tag {};
  struct Def_Base;
  template <typename Drv = Def_Base>
  struct Base : Base_Tag
  { 
    EXCEPT_NAME_RET name() { return Drv::name(); }

    Base(const char* message = "")
    {
    #ifndef NDEBUG
      std::cout << Dbg::VLK_ERRO 
        << " : Exception '" << COL_ORANGE << name() << COL_NO << "' thrown with the message " 
        << "\"" << COL_SALAD << message << COL_NO << "\"\n";
    #endif // !NDEBUG
    }
  };

  struct Def_Base : Base<Def_Base> 
  { 
    EXCEPT_NAME_RET name() { return "Base"; } 
  };
  // ------------------  

  // --- Creation failure ---
  struct Creation_Failure_Tag {};
  struct Def_Creation_Failure;
  template <typename Drv = Def_Creation_Failure>
  struct Creation_Failure : Base<Creation_Failure<Drv>>, Creation_Failure_Tag
  { 
    Creation_Failure(const char* message = "") : Base<Creation_Failure<Drv>>{message} {}
    EXCEPT_NAME_RET name() { return Drv::name(); }  
  };


  struct Def_Creation_Failure : Creation_Failure<Def_Creation_Failure>
  {
    EXCEPT_NAME_RET name() { return "Creation_Failure"; }  
  };

  struct Render_Pass_Creation_Failure : Creation_Failure<Render_Pass_Creation_Failure>
  {
    EXCEPT_NAME_RET name() { return "Render_Pass_Creation_Failure"; }  
  };
  
  struct Shader_Module_Creation_Failure : Creation_Failure<Shader_Module_Creation_Failure>
  {
    EXCEPT_NAME_RET name() { return "Shader_Module_Creation_Failure"; }  
  };
  
  struct Pipeline_Layout_Creation_Failure : Creation_Failure<Pipeline_Layout_Creation_Failure>
  {
    EXCEPT_NAME_RET name() { return "Pipeline_Layout_Creation_Failure"; }  
  };

  struct Debug_Messenger_Creation_Failure : Creation_Failure<Debug_Messenger_Creation_Failure>
  {
    EXCEPT_NAME_RET name() { return "Debug_Messenger_Creation_Failure"; }  
  };

  struct Instance_Creation_Failure : Creation_Failure<Instance_Creation_Failure>
  {
    EXCEPT_NAME_RET name() { return "Instance_Creation_Failure"; }  
  };

  struct Swap_Chain_Creation_Failure : Creation_Failure<Swap_Chain_Creation_Failure>
  {
    EXCEPT_NAME_RET name() { return "Swap_Chain_Creation_Failure"; }  
  };

  struct Logical_Device_Creation_Failure : Creation_Failure<Logical_Device_Creation_Failure>
  {
    EXCEPT_NAME_RET name() { return "Logical_Device_Creation_Failure"; }  
  };

  struct Image_Views_Creation_Failure : Creation_Failure<Image_Views_Creation_Failure>
  {
    EXCEPT_NAME_RET name() { return "Image_Views_Creation_Failure"; }  
  };

  struct Window_Surface_Creation_Failure : Creation_Failure<Window_Surface_Creation_Failure>
  {
    EXCEPT_NAME_RET name() { return "Window_Surface_Creation_Failure"; }  
  };

  struct Graphics_Pipeline_Creation_Failure : Creation_Failure<Graphics_Pipeline_Creation_Failure>
  {
    EXCEPT_NAME_RET name() { return "Graphics_Pipeline_Creation_Failure"; }  
  };

  struct Framebuffer_Creation_Failure : Creation_Failure<Framebuffer_Creation_Failure>
  {
    EXCEPT_NAME_RET name() { return "Framebuffer_Creation_Failure"; }  
  };
  
  struct Command_Pool_Creation_Failure : Creation_Failure<Command_Pool_Creation_Failure>
  {
    EXCEPT_NAME_RET name() { return "Command_Pool_Creation_Failure"; }  
  };

  struct Command_Buffer_Allocation_Failure : Creation_Failure<Command_Buffer_Allocation_Failure>
  {
    EXCEPT_NAME_RET name() { return "Command_Buffer_Allocation_Failure"; }  
  };

  struct Semaphore_Creation_Failure : Creation_Failure<Semaphore_Creation_Failure>
  {
    EXCEPT_NAME_RET name() { return "Semaphore_Creation_Failure"; }  
  };
  // ------------------------

  // --- Acquiring failure ---
  struct Def_Acquiring_Failure;
  template <typename Drv = Def_Acquiring_Failure>
  struct Acquiring_Failure : Base<Acquiring_Failure<Drv>>  
  { 
    Acquiring_Failure(const char* message = "") : Base<Acquiring_Failure<Drv>>{message} {}
    EXCEPT_NAME_RET name() { return Drv::name(); } 
  };

  struct Def_Acquiring_Failure : Acquiring_Failure<Def_Acquiring_Failure>
  {
    EXCEPT_NAME_RET name() { return "Acquiring_Failure"; } 
  };

  struct File_Open_Failure : Acquiring_Failure<File_Open_Failure> 
  { 
    EXCEPT_NAME_RET name() { return "File_Open_Failure"; } 
  };

  struct Begin_Command_Buffer_Failure : Acquiring_Failure<Begin_Command_Buffer_Failure>
  {
    EXCEPT_NAME_RET name() { return "Begin_Command_Buffer_Failure"; } 
  };

  struct Record_Command_Buffer_Failure : Acquiring_Failure<Record_Command_Buffer_Failure>
  {
    EXCEPT_NAME_RET name() { return "Record_Command_Buffer_Failure"; } 
  };

  struct Queue_Submittion_Failure : Acquiring_Failure<Queue_Submittion_Failure>
  {
    EXCEPT_NAME_RET name() { return "Queue_Submittion_Failure"; } 
  };
  // -------------------------

  // --- Missing item ---
  struct Missing_Item_Tag {};
  struct Def_Missing_Item;
  template <typename Drv = Def_Missing_Item>
  struct Missing_Item : Base<Missing_Item<Drv>>, Missing_Item_Tag
  { 
    Missing_Item(const char* message = "") : Base<Missing_Item<Drv>>{message} {}
    EXCEPT_NAME_RET name() { return Drv::name(); }  
  };

  struct Def_Missing_Item : Missing_Item<Def_Missing_Item>     
  {
    EXCEPT_NAME_RET name() { return "Missing_Item"; }                  
  };

  struct Val_Layers_Missing : Missing_Item<Val_Layers_Missing>           
  { 
    EXCEPT_NAME_RET name() { return "Val_Layers_Missing"; }            
  };

  struct Device_Extensions_Missing : Missing_Item<Device_Extensions_Missing>     
  { 
    EXCEPT_NAME_RET name() { return "Device_Extensions_Missing"; }     
  };

  struct Unsupported_Physical_Devices : Missing_Item<Unsupported_Physical_Devices>  
  { 
    EXCEPT_NAME_RET name() { return "Unsupported_Physical_Devices"; }  
  };

  struct File_Missing : Missing_Item<File_Missing>                  
  { 
    EXCEPT_NAME_RET name() { return "File_Missing"; }                  
  };

  struct Environment_Var_Missing : Missing_Item<Environment_Var_Missing>       
  { 
    EXCEPT_NAME_RET name() { return "Environment_Var_Missing"; }       
  };

  struct Shader_Code_Missing : File_Missing           
  { 
    EXCEPT_NAME_RET name() { return "Shader_Code_Missing"; }           
  };
  // --------------------
}
// -------------------------------


