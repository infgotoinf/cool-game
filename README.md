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


_Copyright (c) 2014-2026 Ramon Santamaria ([@raysan5](https://github.com/raysan5))_
-----------------------------------

## HexAgony

### Description

A Game about murdering creatures and hexing people! You are cursed to curse others to survive and must create hexes from whatever you find!

### Features

 - Birds! (and spiders!)
 - Ticking curse timer that hurries you up!
 - An epic art style, by an epic artist

### Controls

- LMB - drag object/swing knife
- RMB - take knife/drop knife

### Screenshots

<img width="652" height="651" alt="image" src="https://github.com/user-attachments/assets/29e243ad-e1e2-4a3f-b077-1d01e09de671" />


### Developers

 - -inf(infgotoinf) - programmer, artist, sound designer, musician
 - DemDanEm - programmer

### Links

 - itch.io Release: https://infgotoinf.itch.io/hexagony

### License

This project sources are licensed under an unmodified zlib/libpng license, which is an OSI-certified, BSD-like license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.

*Copyright (c) 2026 -inf(infgotoinf) & DemDanEm*
