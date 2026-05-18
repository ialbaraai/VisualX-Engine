# VisualX Engine

> A native desktop game engine for building, designing, and running 2D games — built from scratch in C++ using [SDL3](https://github.com/libsdl-org/SDL), [Dear ImGui](https://github.com/ocornut/imgui), and [Lua (sol2)](https://github.com/ThePhD/sol2).

---

## Overview

VisualX Engine is a fully self-contained 2D game engine with an integrated visual editor, a fixed-timestep game runtime, and a Lua scripting layer. Every component is implemented from the ground up — from the entity component system and scene parser to the Lua API bindings and splash loader. Developers can design scenes visually in the editor, attach Lua scripts for game logic, and launch their projects directly from the engine's project hub.

---

## Architecture

VisualX Engine is composed of three distinct runtime modes, all orchestrated by the central `Engine` class:

### Engine (Hub)
The main entry point and project manager. Presents an ImGui-based dashboard listing all saved projects, with tabs for launching a project in Play mode or reopening it in the Editor. Supports creating new projects by filename and persists the project list to `games.txt`.

### Designer (Editor)
A full visual scene editor built on SDL3 and Dear ImGui. Loads and saves scenes using the custom `.vx` file format, parsed by the `Parser` class. Supports:

- Drag-and-drop entity positioning on the preview canvas (Shift + Click to snap)
- Per-entity property editing: name, ID, position, size, RGBA color, and sprite image
- Live scene background color and resolution controls (500–1920 × 500–1080)
- Lua script integration via file path, with auto-generation of a `Start`/`Loop` template if the script doesn't yet exist
- Entity creation and deletion panels
- Scene state serialized back to `.vx` on editor close

### Game (Runtime)
A fixed-timestep game loop at 60 FPS. Loads a parsed scene, registers all entities into the `EntitiesHandler`, and exposes the complete game API to Lua via `sol2`. Runs `Start()` once at launch and `Loop(dt)` on every fixed tick.

---

## Lua Scripting API

Scripts are plain `.lua` files attached to a scene. Two lifecycle hooks are called by the engine:

```lua
function Start()
  -- called once at game launch
end

function Loop(dt)
  -- called every fixed timestep (~1/60s)
end
```

### Global Tables

| Table | Description |
|---|---|
| `Entities` | Table of all scene entities, keyed by name |
| `Keys` | Key code constants (`Keys.Space`, `Keys.ArrowLeft`, `Keys.a`, etc.) |
| `Mouse` | `Mouse.X`, `Mouse.Y` — updated every frame |
| `WindowWidth` / `WindowHeight` | Scene dimensions |

### Entity API

Each entity in `Entities` exposes the following properties:

| Property | Type | Description |
|---|---|---|
| `Name` | `string` | Entity name |
| `Id` | `int` (read-only) | Unique entity ID |
| `IsAlive` | `bool` | Toggle entity existence |
| `Position` | `Position(X, Y)` | World position |
| `Size` | `Size(X, Y)` | Rendered dimensions |
| `Color` | `Color(R, G, B, A)` | Fill color (0–255) |
| `SpriteComponent` | `SpriteComponent` | Image state and path |

### Functions

```lua
-- Entity management
VX_CreateEntity(Id, Name, Position, Size, Color, SpriteComponent) → Entity
VX_FindEntityById(Id) → Entity | nil

-- Input
VX_GetKeyDown(Keys.X)   → bool   -- fired once on press
VX_GetKeyHeld(Keys.X)   → bool   -- true while held
VX_GetKeyUp(Keys.X)     → bool   -- fired once on release

VX_IsMouseLeftDown()    → bool
VX_IsMouseLeftHeld()    → bool
VX_IsMouseLeftUp()      → bool
-- Also: Right, Middle variants

-- Runtime
VX_GetRuntime()         → float   -- seconds since game start
VX_Wait(seconds)        -- blocking delay
VX_Quit()               -- exit the game loop

-- Rendering
VX_SetImage(Id, filepath)         → bool
VX_SetThumbnail(filepath)         → bool

-- FPS helpers
VX_SetFPS(fps)                    -- clamps to 0–360
VX_SixtyFPS()                     → 60.0
VX_OneTwentyFPS()                 → 120.0
-- Also: VX_ThirtyFPS, VX_FourtyEightFPS, VX_OneFourtyFourFPS, VX_TwoFourtyFPS
```

---

## Scene File Format (`.vx`)

VisualX scenes are plain-text files using a simple delimiter-based format parsed by the `Parser` class:

```
800,600,20,20,20          ← WindowWidth, WindowHeight, R, G, B
$/EOWDTED/$               ← end-of-window-data
Player,1,64,64,100,200,255,0,0,0,   ← Name,Id,W,H,X,Y,R,G,B,IsImage,ImagePath
Enemy,2,32,32,400,300,0,255,0,0,
$/EOEDTSFP/$              ← end-of-entity-data
scripts/game.lua          ← Lua script filepath
$/EOGD/$                  ← end-of-game-data
```

---

## Tech Stack

| Component | Technology |
|---|---|
| Language | C++20 |
| Windowing & Rendering | SDL3, SDL3\_image |
| GUI | Dear ImGui (SDL3 + SDLRenderer3 backend) |
| Scripting | Lua via [sol2](https://github.com/ThePhD/sol2) |
| Build System | CMake 3.20+ |

---

## Project Structure

```
VisualX-Engine/
├── application/
│   ├── src/
│   │   ├── application/       # Engine, Designer, Game, Loader
│   │   ├── system/            # Entity, EntityHandler, EntitiesHandler, Parser, Backend
│   │   └── utilities/         # VXLua (sol2 Lua manager)
│   ├── include/               # Header files mirroring src layout
│   ├── main.cpp               # Entry point — initializes Loader then Engine
│   └── CMakeLists.txt
├── assets/                    # Engine icon, Roboto font
└── games.txt                  # Persisted project paths (auto-managed)
```

---

## Building

### Prerequisites

- CMake 3.20 or higher
- A C++20-compatible compiler (GCC 11+, Clang 13+, MSVC 2022+)
- SDL3 and SDL3\_image installed on your system
- Lua 5.x development headers (for sol2)

### macOS (Homebrew)

```bash
brew install sdl3 sdl3_image lua cmake ninja
```

### Linux (pacman)

```bash
sudo pacman -S sdl3 sdl3_image lua cmake ninja
```

### Linux (dnf)

```bash
sudo dnf install SDL3-devel SDL3_image-devel lua-devel cmake ninja-build
```

### Windows (vcpkg)

```bash
git clone https://github.com/microsoft/vcpkg && cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg install sdl3 sdl3-image lua
winget install Kitware.CMake
winget install Ninja-build.Ninja
```

### Windows (MSYS2 MinGW)

```bash
pacman -S mingw-w64-x86_64-SDL3 mingw-w64-x86_64-SDL3_image \
          mingw-w64-x86_64-lua mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja
```

### Build Steps

```bash
git clone https://github.com/ialbaraai/VisualX-Engine.git
cd VisualX-Engine/application
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/VisualXEngine
```

---

## Workflow

1. **Launch** the engine — the splash loader runs, then the project hub opens.
2. **Create** a new project by entering a filename (e.g. `mygame.vx`) and clicking **Create**.
3. **Design** your scene in the editor: set the canvas size and background color, create entities, assign colors or sprite images, and attach a Lua script.
4. **Close** the editor — the scene is automatically saved back to the `.vx` file.
5. **Play** your project from the hub's **Play** tab to launch the game runtime.

---

## Theming

The engine supports three UI themes, configurable at construction time via the `vxm` enum:

| Theme | Value | Description |
|---|---|---|
| `vxm::Dark` | `10001` | Dark background, muted control colors (default) |
| `vxm::Light` | `20002` | Light background, soft gray controls |
| `vxm::None` | `30003` | ImGui Classic style |

---

## License

This project is licensed under the MIT License. See [LICENSE](./LICENSE) for details.

---

## Author

**albaraa** — [@ialbaraai](https://github.com/ialbaraai)
