{
  description = "Diffraction visualization project flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }: {
    overlay = (final: prev: {
      diffraction-visualize = prev.callPackage ./default.nix {};
    });
  } // flake-utils.lib.eachDefaultSystem (system:
    
      let
        pkgs = import nixpkgs {
          inherit system;
          overlays = [ self.overlay ];
        };
      in {
        packages = {
          inherit (pkgs) diffraction-visualize;
        };

        defaultPackage = pkgs.diffraction-visualize;
        
        nixosModules.diffraction-visualize = { pkgs, ... }: {
            nixpkgs.overlays = [ self.overlay ];
        
            environment.systemPackages = [ pkgs.diffraction-visualize ];
        };

        devShells.default = pkgs.mkShell {
          packages = with pkgs; [ pkg-config ];
          inputsFrom = [ pkgs.diffraction-visualize ];
        };

      }
    );
}
