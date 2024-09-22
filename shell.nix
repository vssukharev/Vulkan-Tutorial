{ pkgs ? import <nixpkgs> {} }: with pkgs;
  mkShell {
    packages = [
    ];

    buildInputs = [
      shaderc

      vulkan-headers
      vulkan-loader
      vulkan-tools-lunarg
      vulkan-helper
      vulkan-tools
      vulkan-validation-layers

      libGL
      glfw
      glfw2
      glfw-wayland

      wayland
      
      glm
    ];

    nativeBuildInputs = [
      pkg-config
    ];

    shellHook = ''zsh'';

    # If it doesn’t get picked up through nix magic
    VK_LAYER_PATH = "${vulkan-validation-layers}/share/vulkan/explicit_layer.d"; 
    VK_DRIVER_FILES=/run/opengl-driver/share/vulkan/icd.d/nvidia_icd.x86_64.json;
    # VULKAN_SDK = "${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d";

    # vulkan-loader = vulkan-loader;
    # glfw2 = glfw2;
    # glfw = glfw;
  }
