# Achilles: The Loom of Fate

A story-driven terminal roguelike about the greatest Greek warrior and the one place a blade can kill him.

You are Achilles. You are invulnerable everywhere except your heel. A single heel strike ends the run. Every death cuts a thread; every run weaves it again. Kleos, memory fragments, weapons, and hard mode persist between runs.

Built with C++17, ncursesw, and **STARKDB** for saves.

---

## Requirements

- Linux (primary target)
- A C++17 compiler and CMake 3.16+
- Wide-character ncurses (`ncursesw`)
- **STARKDB** core library + C++ header (`libstark.so`, `stark.h`, `stark.hpp`)

### Install common packages

Debian / Ubuntu:

```bash
sudo apt update
sudo apt install build-essential cmake libncurses-dev
```

On older releases the package may be named `libncursesw5-dev` instead.

Fedora:

```bash
sudo dnf install gcc-c++ cmake ncurses-devel
```

Arch:

```bash
sudo pacman -S base-devel cmake ncurses
```

---

## If you already have STARKDB installed

If `libstark.so` is in a standard library path and `stark.hpp` / `stark.h` are in a standard include path:

```bash
cmake -S . -B build
cmake --build build -j
./build/achilles
```

That is the normal path. Continue to the next section only if CMake cannot find STARKDB.

---

## If you do not have STARKDB

STARKDB is a lightweight embedded key-value database. The game only needs its C core and C++ header.

### Option A: install STARKDB system-wide (recommended)

```bash
git clone https://github.com/abdullahtnz/starkdb.git
cd starkdb

cmake -S . -B build -DBUILD_SHARED=ON
cmake --build build -j

sudo make -C build install
sudo ldconfig
```

Some STARKDB builds do not install the C++ adapter header. If `stark.hpp` is missing afterward, copy it manually:

```bash
sudo cp bindings/cpp/stark.hpp /usr/local/include/
sudo cp core/include/stark.h       /usr/local/include/
```

Then build the game:

```bash
cd /path/to/Achilles-LOF
cmake -S . -B build
cmake --build build -j
./build/achilles
```

### Option B: keep STARKDB local (no system install)

This keeps the database library inside the game folder.

```bash
# From a directory containing both starkdb/ and Achilles-LOF/

# Build STARKDB first if you have not already:
cmake -S starkdb -B starkdb/build -DBUILD_SHARED=ON
cmake --build starkdb/build -j

mkdir -p Achilles-LOF/third_party/stark/include
mkdir -p Achilles-LOF/third_party/stark/lib

cp starkdb/bindings/cpp/stark.hpp Achilles-LOF/third_party/stark/include/
cp starkdb/core/include/stark.h   Achilles-LOF/third_party/stark/include/
cp starkdb/build/libstark.so      Achilles-LOF/third_party/stark/lib/

cd Achilles-LOF

cmake -S . -B build \
  -DSTARK_INC="$PWD/third_party/stark/include" \
  -DSTARK_LIB="$PWD/third_party/stark/lib/libstark.so"

cmake --build build -j

LD_LIBRARY_PATH="$PWD/third_party/stark/lib:$LD_LIBRARY_PATH" ./build/achilles
```

If the game runs but cannot load `libstark.so`, use the `LD_LIBRARY_PATH` prefix shown above.

---

## How to play

Run:

```bash
./build/achilles
```

On first launch you get the prologue tutorial. Press `ENTER` to move through it. After that, the main menu appears.

### Main menu

| Key | Action |
|---|---|
| `1` | New run, or continue the run in progress |
| `2` | Abandon the current run and start a new one (only when a run exists) |
| `3` | Enter the Loom (meta-progression / spend kleos) |
| `4` | Quit |
| `5` | Toggle THE NAMELESS THREAD hard mode (only after unlocking it) |
| `?` | Open the manual |

### Dialogue

| Key | Action |
|---|---|
| `ENTER` | Advance to the next line |
| `1`–`9` | Choose a dialogue option when choices appear |
| `P` | Pause |

### Combat

| Key | Action |
|---|---|
| `A` | Attack |
| `R` | Reckless strike — spends HP for heavy damage (Layer 3+) |
| `F` | Focus — automatically dodges the next attack (Layer 2+) |
| `D` | Dodge |
| `H` | Heal |
| `S` | Cycle stance (Layer 2+) |
| `X` | Flee |
| `P` | Pause |

### The one rule: protect the heel

Most damage is survivable. A heel strike is not.

When an enemy attacks, a coil meter appears. The meter turns **green** near the end of the wind-up. That green window is when you should press `D`. Pressing too early wastes stamina and the dodge expires.

Ways to protect the heel:

- Dodge inside the green window.
- Use Focus to read the next attack without timing.
- Switch to Defensive stance: it negates heel strikes.
- Spend a heel-guard token. Tokens come from the Odysseus choice in Chapter 2 and from Thetis at the Loom.

### Stances

- **Balanced** — baseline.
- **Aggressive** — more damage, more risk.
- **Defensive** — less damage dealt, less taken, and heel strikes are negated.

### Kleos and the Loom

Kleos is glory. It persists across deaths. You earn it from fights and from dying.

At the Loom you can:

- Unlock the next weapon: spear, sword, shield, chariot, bow. The newest weapon is carried into future runs and changes your damage.
- Buy a Thetis heel-guard token. Tokens carry into the next run and survive one heel strike each.
- Read the Fates' commentary and review collected memory fragments and witnessed endings.

Memory fragments are awarded at the end of each chapter. Collect all six to unlock the true ending at the final gate.

---

## Saving

The game auto-saves through STARKDB after actions, scene changes, combat starts, and deaths.

Save location:

```
~/.achilles/
  save.idx
  save.dat
  backup.idx
  backup.dat
```

Only one instance of the game can open the save database at a time. If you see a database-lock error, close other copies of the game.

To reset all progress, delete the save folder:

```bash
rm -rf ~/.achilles
```

### New-player stats

Yes — a brand-new save starts at zero:

- kleos: `0`
- deaths: `0`
- runs: `0`
- unlocked weapons: spear only
- memory fragments: none
- endings seen: none

`runs` becomes `1` as soon as you start your first run. If you already have a save, the game loads those older values; delete `~/.achilles` for a clean profile.

---

## Endings

There are three:

- **A — KLEOS**: the default. Die at Troy, or accept the arrow at the gate.
- **B — THE PHANTOM**: die repeatedly on the path of rage with grief unresolved. Unlocks hard mode.
- **C — THE TRUE ENDING**: collect all six memory fragments, then refuse Apollo's framing at the final gate.

The manual in-game (`?` from the menu, `M` from the Loom) explains hints without spoiling every scene.

---

## Troubleshooting

### CMake cannot find STARKDB

Point CMake at the exact files:

```bash
cmake -S . -B build \
  -DSTARK_INC=/path/to/include-folder-containing-stark.hpp \
  -DSTARK_LIB=/path/to/libstark.so
```

Both `stark.hpp` and `stark.h` must be in the include folder.

### `error while loading shared libraries: libstark.so`

Either install it system-wide and run `sudo ldconfig`, or run with:

```bash
LD_LIBRARY_PATH=/path/to/starkdb/build:$LD_LIBRARY_PATH ./build/achilles
```

### Garbled characters or missing box-drawing glyphs

Use a UTF-8 locale and a modern terminal:

```bash
export LANG=C.UTF-8
export TERM=xterm-256color
```

A terminal of at least roughly `80x24` is required; `100x30` or larger is recommended.

### `Error opening terminal: unknown`

Set a terminal type before running:

```bash
TERM=xterm-256color ./build/achilles
```

### Database lock error

Another game process is using `~/.achilles`. Close it and try again.

---

## License

MIT. See [LICENSE](LICENSE).
