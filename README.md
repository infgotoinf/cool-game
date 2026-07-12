## Build + Run

1. Clone project's repo:
```bash
git clone https://github.com/infgotoinf/cool-game
```

2. Install git modules:
```bash
git submodule update --init --recursive
```

2.1 If you are on NixOS:
```bash
nix-shell
```

2.2 If you use [devbox.json](https://www.jetify.com/devbox):
```bash
devbox shell
```

3. Install [emsdk](https://github.com/emscripten-core/emsdk):

> [!NOTE]
> **You need to have Python installed**. If something doesn't work, you should probably follow [Raylib's web setub guide](https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)#4-setup-raylib-game-for-web).

  ```bash
  cd vendored/emsdk
  ./emsdk install latest
  ./emsdk activate latest
  ```

4. Build for web:

  ```bash
  cd ../..
  vendored/emsdk/upstream/emscripten/emcmake cmake -B build
  cmake --build build
  ```
  <!-- - For desktop: -->
  <!-- ```bash -->
  <!-- cd ../../src -->
  <!-- PLATFORM=PLATFORM_DESKTOP make -->
  <!-- ``` -->

5. Run in browser:

  ```bash
  vendored/emsdk/upstream/emscripten/emrun build/game/game.html
  ```

  <!-- - Desktop: -->
  <!-- ```bash -->
  <!-- build/game -->
  <!-- ``` -->


> [!NOTE]
> I left the next build sections just for the reference if something doesn't work. You probably don't need them.

#### Windows: Visual Studio

- After extracting the zip, the parent folder `raylib-game-template` should exist in the same directory as `raylib` itself.  So, your file structure should look like this:
    - Some parent directory
        - `raylib`
            - the contents of https://github.com/raysan5/raylib
        - `raylib-game-template`
            - this `README.md` and all other raylib-game-template files
- If using Visual Studio, open projects/VS2022/raylib-game-template.sln
- Select on `raylib_game` in the solution explorer, then in the toolbar at the top, click `Project` > `Set as Startup Project`
- Now you're all set up!  Click `Local Windows Debugger` with the green play arrow and the project will run.

#### Linux

When setting up this template on linux for the first time, install the dependencies from this page:
([Working on GNU Linux](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux))

You can use this templates in a few ways: using Visual Studio, using CMake, or make your own build setup. This repository comes with Visual Studio and CMake already set up.

Chose one of the follow setup options that fit in you development environment.

### CLI: Makefile

```sh
mkdir ~/raylib-gamejam && cd ~/raylib-gamejam
git clone --depth 1 --branch 6.0 https://github.com/raysan5/raylib
make -C raylib/src
git clone https://github.com/$(User Name)/$(Repo Name).git
cd $(Repo Name)
make -C src
src/raylib_game
```

This template has been created to be used with raylib (www.raylib.com) and it's licensed under an unmodified zlib/libpng license.

_Copyright (c) 2014-2026 Ramon Santamaria ([@raysan5](https://github.com/raysan5))_
-----------------------------------

## $(Game Title)

![$(Game Title)](screenshots/screenshot000.png "$(Game Title)")

### Description

A Game about catching birds and hexing people! You are cursed to curse others to survive and must create hexes from whatever you find! 

### Features

 - Birds!
 - Ticking curse timer that hurries you up!
 - An epic art style, by an epic artist

### Controls

Keyboard:
 - All Mouse all the time

### Screenshots

<img width="652" height="651" alt="image" src="https://github.com/user-attachments/assets/29e243ad-e1e2-4a3f-b077-1d01e09de671" />


### Developers

 - infgotoinf - main code, art, audio
 - demdanem - additional code

### Links

 - YouTube Gameplay: $(YouTube Link)
 - itch.io Release: $(itch.io Game Page)
 - Steam Release: $(Steam Game Page)

### License

This project sources are licensed under an unmodified zlib/libpng license, which is an OSI-certified, BSD-like license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.

$(Additional Licenses)

*Copyright (c) $(Year) $(User Name) ($(User Twitter/GitHub Name))*
