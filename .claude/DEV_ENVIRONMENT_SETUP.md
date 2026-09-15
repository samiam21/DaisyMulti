# Daisy Dev Environment Setup (Windows) — Reinstall Walkthrough

Last verified against: **TheFirmament** repo state as of 2026-06-24 (latest commit `0bd4388`),
cross-checked against **docs.daisy.audio** (the current official docs site) as of 2026-07-01.

**Important — the old `github.com/electro-smith/DaisyToolchain` repo is dead.** Its last
commit and last release (`v0.3.3`) are both from May 2022. Electrosmith consolidated all
their docs (previously scattered across GitHub Wiki, the shop site, and the forum) into a
new unified site at **`docs.daisy.audio`**, and the installer itself moved off GitHub
entirely onto a CDN. Use the links below, not the GitHub releases page.

Good news: your project folders on `D:\Workspace\Daisy` (TheFirmament, DaisyMulti, etc.) are
still intact with their git history and submodules — a Windows reinstall only wipes
machine-level software installs (`C:\Program Files\...`), not files on the `D:` drive.
So **you do not need to re-clone anything** — you just need to reinstall the tools below.

## What you need to (re)install

1. **Git for Windows** — https://gitforwindows.org/ (gives you Git Bash, which the current
   docs use as the default terminal for building)
2. **VS Code** — https://code.visualstudio.com/
3. **DaisyToolchain v1.1.0** (current — bundles Git, `make`, the ARM `arm-none-eabi` compiler,
   and `dfu-util`; OpenOCD/clang-format/Python/Doxygen are now called out as optional extras,
   not part of the core bundle):
   **direct installer link:** https://daisy.nyc3.cdn.digitaloceanspaces.com/installers/DaisyToolchain-1.1.0-win64.exe
   (linked from https://docs.daisy.audio/tutorials/toolchain-windows/)
4. **SEGGER J-Link software** — https://www.segger.com/downloads/jlink/ (TheFirmament debugs
   via J-Link, not ST-Link/OpenOCD)
5. **VS Code extensions**: `C/C++` (Microsoft) and `Cortex-Debug` (marus25)
6. **Python 3.8+** (optional, for helper scripts) — get it from python.org directly, **not**
   the Microsoft Store version (Store version causes a PATH-ordering conflict — see
   Troubleshooting below)
7. **Zadig** (only if `dfu-util` can't see the board in bootloader mode) —
   https://zadig.akeo.ie/

## Step-by-step

### 1. Install Git for Windows, then VS Code
Standard installers, default options are fine. In VS Code, switch the default integrated
terminal to **Git Bash** (current official docs call this out explicitly — click the
dropdown next to the `+` in the terminal panel → Select Default Profile → Git Bash).

### 2. Install the DaisyToolchain (v1.1.0, from the CDN — not the old GitHub releases page)
Download and run:
https://daisy.nyc3.cdn.digitaloceanspaces.com/installers/DaisyToolchain-1.1.0-win64.exe

It installs to **`C:\Program Files\DaisyToolchain\`** — this exact path is hardcoded into
TheFirmament's `.vscode/settings.json` and `.vscode/c_cpp_properties.json`, so keep the
default location unless you're prepared to edit those files.

Verify after install (PowerShell or Git Bash):
```
"C:\Program Files\DaisyToolchain\bin\arm-none-eabi-gcc.exe" --version
"C:\Program Files\DaisyToolchain\bin\make.exe" --version
```
TheFirmament was last built against **arm-none-eabi 10-2020-q4-major** (GCC 10.2.1) when it
switched to this toolchain path (commit `40f4212`, 2026-05-14) — the 1.1.0 installer may ship
a newer GCC than that; if `make` fails with new warnings/errors that didn't happen before,
that version drift is the first thing to suspect.

Add the toolchain to your PATH (so plain `make`/`arm-none-eabi-gcc` work from any terminal):
```
setx PATH "%PATH%;C:\Program Files\DaisyToolchain\bin"
```
(Restart your terminal/VS Code after this.)

### 3. Install SEGGER J-Link software
Download "J-Link Software and Documentation Pack" for Windows and install with defaults.
This provides `JLinkGDBServerCL.exe`, which each project's `.vscode\launch.json` points at
directly for debugging.

**Note the versioned install path.** J-Link installs into a version-numbered folder — e.g.
`C:\Program Files\SEGGER\JLink_V956\JLinkGDBServerCL.exe`, *not* a stable unversioned
`...\SEGGER\JLink\` path. After any J-Link update, check `C:\Program Files\SEGGER\` for the
new folder name and update `serverpath` in every project's `.vscode/launch.json` to match,
or F5 will fail before it ever reaches the board.

### 4. Install VS Code extensions
In VS Code: Extensions tab → search **"Cortex-Debug"** (publisher `marus25`) → Install.
Also install Microsoft's **C/C++** extension if not already present (IntelliSense).

### 5. (Optional) Install Python 3.8+
Only needed for helper/build scripts. Install from https://python.org directly — **avoid**
the Microsoft Store version. Verify with `python --version`. If `python` isn't found despite
installing, or the Store version shadows it: open "Edit the system environment variables" →
Environment Variables → edit the user `Path` → move your real Python path (e.g.
`C:\Program Files\Python310`) above any Microsoft Store `WindowsApps` entries → restart terminals.

### 6. Sanity-check the existing project (no re-clone needed)
Open a terminal in `D:\Workspace\Daisy\TheFirmament` and confirm submodules are intact:
```
git status
git submodule status
```
If any submodule shows as uninitialized/dirty from the reinstall, run:
```
git submodule update --init --recursive
```

### 7. Build
**Must run in a Git Bash terminal, not PowerShell or cmd.exe.** Make's recipes (e.g.
`rm -fR build`) are POSIX shell commands; Make needs `sh.exe` on PATH to run them, which
PowerShell doesn't provide. Running `make clean` in PowerShell fails with:
```
process_begin: CreateProcess(NULL, rm -fR build, ...) failed.
make (e=2): The system cannot find the file specified.
```
In VS Code, switch the terminal profile to **Git Bash** (dropdown next to the `+` in the
terminal panel → select Git Bash, or "Select Default Profile" if it's not listed), then:
```bash
cd /d/Workspace/Daisy/TheFirmament
make clean
make
```
Or in VS Code: Command Palette → **Tasks: Run Task** → `build` (VS Code tasks run through
the configured shell already, so this sidesteps the issue). This should produce
`build/TheFirmament.elf` and `build/TheFirmament.bin`. If `make` isn't found at all, your
PATH edit from step 2 didn't take — restart the terminal, or use the full path to `make.exe`.

If you see missing static libs, build the vendored libraries first (only needed after a
fresh submodule checkout — normally cached under `lib/libdaisy/build` and `lib/DaisySP/build`):
```
cd lib\libdaisy && make && cd ..\..
cd lib\DaisySP && make && cd ..\..
```

### 8. Flash via USB (DFU) — simplest path, no extra hardware
Put the Daisy Seed in bootloader mode: **hold BOOT, tap RESET, release BOOT**.
Then:
```
make program-dfu
```
Or VS Code task `build_and_program_dfu`.

**Windows gotcha:** if `dfu-util` reports it can't find the device, run **Zadig**:
put the Daisy in bootloader mode (hold BOOT, tap RESET, release both), open Zadig →
Options → check **"List All Devices"** → select **"DFU in FS mode"** from the dropdown →
set target driver to **WinUSB** → click **Replace Driver**. (A "(NONE)" label on the left
side during this is normal, not an error.) One-time fix per machine.

### 9. Debug via J-Link in VS Code (optional)
TheFirmament already ships a working debug config — nothing to author from scratch:
- `.vscode/launch.json` — points at `JLinkGDBServerCL.exe`, target `STM32H750XB`, interface `SWD`
- `.vscode/Daisy.JLinkScript` — **required**: the Daisy Seed doesn't expose the NRST pin, so
  this script does a software reset via the Cortex-M7 AIRCR register instead of hardware reset

Wire a J-Link probe to the Seed's SWD pins, then just press **F5** in VS Code (it will
build first via the `preLaunchTask`).

**Two hardware gotchas — both look like config/wiring faults but aren't:**

1. **Power the board separately.** The J-Link probe carries debug signals (SWDIO/SWCLK/GND)
   only; it does *not* power the target. With no USB/5V applied to the Seed, J-Link connects
   to the probe fine but reports `Connecting to target failed. Connected correctly?`.
2. **Tap the Seed's RESET button right before pressing F5.** Because NRST isn't wired to the
   J-Link header, J-Link cannot reset-and-halt a target that is already running firmware — it
   can only attach to the chip's current state. A freshly-powered board attaches fine, but once
   your firmware is running (480 MHz, audio ISRs, QSPI memory-mapped) the connect fails with
   that same "Connecting to target failed" message. So the second and every subsequent debug
   session needs a RESET tap (or a power-cycle) first. Note `Daisy.JLinkScript`'s
   `ResetTarget()` can't help here — it only runs *after* a successful connect.

   If this ever becomes too annoying, the permanent fixes are to wire NRST to pin 15 of the
   J-Link header, or extend `InitTarget()` in `Daisy.JLinkScript` to do an SWD-only
   connect-under-reset (halt via `DEMCR.VC_CORERESET`, then issue `SYSRESETREQ`).

To diagnose a connect failure in more detail, run the J-Link CLI directly — it distinguishes
"sees nothing at all" (power/wiring) from "sees the debug port but can't halt the core" (chip state):
```
"C:\Program Files\SEGGER\JLink_V956\JLink.exe" -device STM32H750XB -if SWD -speed 1000 -autoconnect 1
```

## Applying this to DaisyMulti (and other pedals)
Steps 1–5 are machine-wide and only need doing once. For each other project folder
(`DaisyMulti`, `TheCommander`, `TheMartian`, `TheFirmament`, `TerrariumTemplate`), just
repeat steps 6–9 inside that folder. If a project's `.vscode` config still points at the
*old* toolchain path (`C:\Program Files\Arm\GNU Toolchain mingw-w64-x86_64-arm-none-eabi\bin`
— the pre-2026-05-14 path TheFirmament itself used to use), update
`cortex-debug.armToolchainPath` in `.vscode/settings.json` and `compilerPath` in
`.vscode/c_cpp_properties.json` to `C:/Program Files/DaisyToolchain/bin` to match.

## Reference: what's bundled in DaisyToolchain v1.1.0 (current, per docs.daisy.audio)
**Core (installed by the installer above):**
- Git
- GNU Make
- ARM `arm-none-eabi` cross-compiler toolchain
- dfu-util

**Optional/advanced (not installed by default — install separately only if you need them):**
- OpenOCD (not needed if you debug via J-Link, as TheFirmament does)
- clang-format
- Python 3.0+ (helper scripts)
- Doxygen (doc generation)
- Pandoc

This is a change from the old (2022, GitHub-era) toolchain bundle, which used to include
OpenOCD and clang-format by default alongside Make/dfu-util/ARM-gcc at fixed pinned versions
(Make 4.3, ARM 10-2020-q4-major, OpenOCD 0.11.0, dfu-util 0.10, clang-format 10.0.0). The
CDN-hosted v1.1.0 installer may ship newer tool versions — treat the old pinned numbers as
historical context, not a guarantee of what you'll get today.

## Sources (docs.daisy.audio is the current, authoritative site — verified 2026-07-01)
- [docs.daisy.audio — Windows Toolchain Install](https://docs.daisy.audio/tutorials/toolchain-windows/) (current installer link + Git/Python prerequisites)
- [docs.daisy.audio — C++ Dev Env Getting Started](https://docs.daisy.audio/tutorials/cpp-dev-env/)
- [docs.daisy.audio — Understanding the Toolchain](https://docs.daisy.audio/tutorials/Understanding-the-Toolchain/) (what's core vs. optional)
- [docs.daisy.audio — Zadig USB driver reset](https://docs.daisy.audio/tutorials/zadig/)
- [The New Daisy Support Site Is Live (blog)](https://daisy.audio/blogs/seeds-n-circuits/the-new-daisy-support-site-is-live) — explains the docs migration off GitHub Wiki/forum
- [electro-smith/DaisyToolchain](https://github.com/electro-smith/DaisyToolchain) — **stale/archived reference only**, last commit/release May 2022, do not use for downloads
- [electro-smith/DaisyExamples README](https://github.com/electro-smith/DaisyExamples/blob/master/README.md) — still current for clone/build/flash command reference
- [Daisy Web Programmer](https://flash.daisy.audio/) (browser-based fallback flashing tool if `dfu-util`/Zadig gives trouble)
- [Debug Daisy with SEGGER J-Link EDU in VS Code — Daisy Forums](https://forum.electro-smith.com/t/debug-daisy-with-segger-j-link-edu-in-visual-studio-code/1548)
- Marus/cortex-debug VS Code extension
