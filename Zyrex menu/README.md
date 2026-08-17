# Zyrex Menu (Vision v2.0.0)

External Roblox overlay menu. Reads/writes Roblox memory externally via DX11 transparent overlay + Dear ImGui. Runs as a standalone `.exe` — no DLL injection required.

---

## Quick Start

### Requirements
- **Visual Studio 2022** (MSVC v143 toolset, Windows 10 SDK, MASM x64)
- **Windows 10/11 x64**
- **Administrator** privileges (enforced via manifest)

### Build
1. Open `ImGui.sln` in VS2022
2. Set config to **Release | x64**
3. Build Solution (`Ctrl+Shift+B`)
4. Output: `bin/menu.exe`

### Run
1. Launch Roblox and join a game
2. Run `bin/menu.exe` **as Administrator**
3. Menu attaches automatically — press **F1** to toggle the overlay

---

## Project Structure

```
Zyrex menu/
├── ImGui.sln                          # VS solution (menu + loader)
├── bin/                               # Build output
├── vendors/                           # Pre-compiled libs (DirectX, FreeType, Boost)
│
├── projects/
│   ├── shared/                        # Shared framework (window, DX11, ImGui, fonts, textures)
│   │   ├── Application.{h,cpp}        # Base app class (init, message loop, shutdown)
│   │   ├── Console.{h,cpp}            # In-app logging (Info/Debug/Warn/Error)
│   │   ├── Direct3D11.{h,cpp}         # DX11 device + swap chain setup
│   │   ├── Window.{h,cpp}             # Win32 transparent overlay window
│   │   ├── ImGui/                     # Dear ImGui core + DX11/Win32 backends
│   │   └── Resources/                 # Embedded fonts (9) and icons (20+)
│   │
│   ├── loader/                        # Stub loader (placeholder, not used)
│   │   ├── Main.cpp
│   │   └── Menu.cpp                   # Displays "This offer does not include a loader"
│   │
│   └── menu/                          # Main menu project
│       ├── Main.cpp                   # Entry point: overlay, DX11, Roblox attach, main loop
│       ├── Menu.cpp                   # ImGui UI: tabs, styling, feature panels
│       ├── app.manifest               # Requires Administrator
│       │
│       ├── src/
│       │   ├── settings.h             # ALL settings/defaults (769 lines)
│       │   │
│       │   ├── memory/                # Process memory access
│       │   │   ├── memory.{h,cpp}     # RPM/WPM wrappers + process attach/detach
│       │   │   └── luck.asm           # x64 assembly: kernel-mode-style RPM/WPM
│       │   │
│       │   ├── sdk/                   # Roblox SDK
│       │   │   ├── sdk.{h,cpp}        # instance_t, player_t, part_t, humanoid_t, camera_t
│       │   │   └── math/math.h        # Vector2/3, Matrix3/4, world-to-screen
│       │   │
│       │   ├── Offsets/               # Roblox memory offsets
│       │   │   ├── Offsets.hpp         # Compiled-in offsets (390)
│       │   │   ├── OffsetLoader.hpp    # Load from offsets.json at runtime
│       │   │   ├── AutoUpdater.{h,cpp} # Fetch latest offsets from API
│       │   │   └── FFlags.hpp          # Fast Flag definitions
│       │   │
│       │   ├── game/                  # Game state detection
│       │   │   ├── game.{h,cpp}       # Datamodel, players, workspace, camera
│       │   │   └── rescan/            # Auto-rescan on game changes
│       │   │
│       │   ├── cache/                 # Player/entity caching
│       │   │   ├── cache.{h,cpp}      # entity_t: name, health, team, parts, rig
│       │   │   ├── bodyparts/         # Body part caching
│       │   │   └── custom_entities/   # Custom entity tracking
│       │   │
│       │   ├── menu/                  # Menu internals
│       │   │   ├── keybind/           # Keybind system (toggle/hold/always)
│       │   │   └── rardar/            # Radar display
│       │   │
│       │   ├── gamesupport/           # Per-game detection (Phantom Forces, MM2, LT2, etc.)
│       │   │
│       │   └── features/
│       │       ├── football/football.{h,cpp}  # Football auto-dive system (~1400 lines)
│       │       ├── config/config.{h,cpp}      # Config save/load (JSON)
│       │       ├── aimbot/aimbot.h
│       │       ├── silentaim/silentaim.h
│       │       ├── esp/esp.h + parser/meshparser.h
│       │       ├── movement/ (fly, speed, jump, tickrate, gravity)
│       │       ├── lighting/ (fog, shadows, clocktime, skybox, exposure)
│       │       ├── rage/ (desync, magicbullet, noclip, spin360, rapidfire, hitsounds)
│       │       ├── exploits/ (headless, korblox, antiafk, freezeplayer, fpscaps)
│       │       ├── lua_executor/lua_executor.h
│       │       ├── infinite_ammo/infinite_ammo.h
│       │       └── explorer/explorer.h
│       │
│       └── ext/                       # Embedded libraries
│           ├── imgui/                 # Dear ImGui (copy)
│           ├── zstd/                  # Zstandard compression
│           ├── blake3/                # BLAKE3 hashing
│           ├── xxhash/                # xxHash
│           ├── json/                  # nlohmann/json
│           ├── libcurl.lib            # HTTP client (offset auto-updater)
│           ├── dicons/                # Dex icons + explorer textures
│           ├── font/                  # Embedded fonts (Arial, Tahoma, SP7, Exo2)
│           └── hitsounds.h            # Embedded hit sounds
```

---

## How It Works

### Overlay Architecture
1. **Main.cpp** creates a transparent, topmost, click-through window (`WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE`)
2. Initializes DX11 swap chain + ImGui context
3. Spawns a background thread that waits for `RobloxPlayerBeta.exe`, then attaches via external RPM/WPM
4. Each frame: reads game memory → updates game state → draws ImGui UI

### Memory Access
- Custom x64 assembly in `luck.asm` provides kernel-style `ReadProcessMemory`/`WriteProcessMemory` wrappers
- All Roblox memory access is external (no injection) — reads/writes process memory from outside
- Offsets auto-download from `https://offsets.imtheo.lol/` on first run

### Football System (`features/football/football.cpp`)
The core football auto-dive feature:
- **Ball tracking**: reads `Workspace.BallFolderServer.ballEntity` position/velocity, picks fastest ball
- **Physics simulation**: predicts ball trajectory with configurable gravity, bounce, rolling friction
- **Zone system**: 5 columns (LEFT/MID_LEFT/MIDDLE/MID_RIGHT/RIGHT) × 2 rows (TOP/BOTTOM) = 10 zones
- **Autodive**: automatically presses jump + direction key when ball enters a zone
- **Rotted dive**: holds A/D direction key then taps C (leap key)
- **Auto M2**: auto right-click on ball approach
- **Panel visualization**: camera-relative 3D projected zone display
- **Post guard**: redirects zones near goalposts

### Input System
All input is sent via `SendInput` with minimal latency:
- `tap_key()`: press + sleep(1ms) + release
- Features use `GetAsyncKeyState()` polling in a tight loop (no sleep in hot paths)

---

## Settings

All defaults live in `src/settings.h`. Organized into namespaces:

| Namespace | Description |
|---|---|
| `settings::football` | Auto dive, auto M2, rotted dive, zones, physics, ball tracking |
| `settings::aimbot` | Aimbot with FOV, smoothing, prediction, checks |
| `settings::silentaim` | Silent aim (camlock + mouse spoofing) |
| `settings::rage` | Noclip, desync, magic bullet, spin360, rapid fire, hitsounds |
| `settings::visuals` | ESP (box, name, health, chams, skeleton), radar |
| `settings::movement` | Speed, fly, jump power, gravity, tick rate |
| `settings::lighting` | Fog, shadows, clock time, skybox, exposure |
| `settings::exploits` | Anti-AFK, freeze player, FPS cap |
| `settings::menu` | Keybind, streamproof, vsync, frame limiter, watermark |

---

## Key Files Reference

| File | Lines | Purpose |
|---|---|---|
| `projects/menu/Main.cpp` | ~700 | Entry point, overlay loop, DX11 setup, Roblox attach |
| `projects/menu/Menu.cpp` | ~540 | All ImGui UI — tabs, widgets, feature panels |
| `projects/menu/src/settings.h` | ~770 | Every setting as `inline` globals with defaults |
| `projects/menu/src/features/football/football.cpp` | ~1400 | Football auto-dive, ball tracking, physics, input |
| `projects/menu/src/memory/memory.h` | — | RPM/WPM API (`memory_t` class) |
| `projects/menu/src/sdk/sdk.h` | — | Roblox SDK types (instance, player, part, humanoid) |
| `projects/menu/src/Offsets/Offsets.hpp` | ~390 | Roblox memory offset definitions |
| `projects/menu/src/features/config/config.cpp` | — | Save/load settings to JSON |
| `projects/menu/src/menu/keybind/keybind.h` | — | Keybind enum (toggle/hold/always) + ImGui widget |

---

## Build Details

| Setting | Value |
|---|---|
| Toolset | v143 (MSVC 17.x) |
| C++ Standard | C++20 |
| Runtime | `/MT` (static CRT) |
| Output | `bin/menu.exe` |
| Linked libs | `d3d11.lib`, `d3dx11.lib`, `freetype.lib` |
| Preprocessor | `WIN32`, `NCONSOLE`, `_CRT_SECURE_NO_WARNINGS`, `NOMINMAX` |
| Force include | `force_inline.h` (brings `min`/`max`/`abs` into global scope) |
| MASM | Enabled for `luck.asm` (kernel-style RPM/WPM) |
| Manifest | Requires Administrator elevation |

---

## License

Private — for authorized use only.
