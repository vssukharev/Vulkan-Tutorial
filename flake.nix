{
  description = "Vulkan tutorial flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }: 
  let 
    system = "x86_64-linux";
    pkgs = import nixpkgs { inherit system; };
  in 
  {
    packages.${system} = {
      default = pkgs.mkShell { 
        shellHook = ''echo "Hello Flake!"'';
      };
    };
  };
}
