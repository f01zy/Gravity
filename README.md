# Gravity

## Building
To get started, clone this repository with all the submodules:

```bash
git clone --recurse-submodules https://github.com/f01zy/Gravity gravity && cd gravity
```

If you have NixOS then run the nix-shell to load wayland and OpenGL dependencies (currently, only Wayland build is supported):

```bash
nix-shell
```

And now build the project:

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Preview
![preview](resources/preview.jpg)

## Architecture
![architecture](resources/architecture.png)
