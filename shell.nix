{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    packages = with pkgs; [
      vulkan-helper
      vulkan-tools
    ];

    buildInputs = with pkgs; [
      shaderc

      vulkan-headers
      vulkan-loader
      vulkan-validation-layers # maybe?
      vulkan-tools-lunarg
      renderdoc

      libGL
      glfw
      glfw2
      glfw-wayland

      wayland
      
      glm
    ];

    shellHook = ''zsh'';

    # If it doesn’t get picked up through nix magic
    VK_LAYER_PATH = "${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d"; 
    VK_DRIVER_FILES=/run/opengl-driver/share/vulkan/icd.d/nvidia_icd.x86_64.json;
    # VULKAN_SDK = "${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d";
  }
