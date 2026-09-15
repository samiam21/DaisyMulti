# Daisy Workspace — Project Memory

Last updated: 2026-09-15

This file is manually maintained project memory for the `D:\Workspace\Daisy` workspace,
stored in-repo (rather than only in the user-profile `~/.claude` memory store) so it
survives a Windows reinstall. Update it as work progresses.

**Location note:** this file and `DEV_ENVIRONMENT_SETUP.md` live in `DaisyMulti/.claude/`
and are committed to the DaisyMulti repo. They previously sat in `D:\Workspace\Daisy\.claude\`,
which is *not* a git repo — so despite the stated intent above they were never actually
version-controlled. Moved here 2026-09-15 so a clone on any machine carries them. They still
describe the whole workspace, not just DaisyMulti.

## Status log

**2026-09-15 — DaisyMulti library upgrade complete, verified on hardware.** Committed as
`84c097b "Update libraries"` on `develop` and pushed. Submodules now: `libdaisy` **v8.1.0**
(was v4.0.0+15), `DaisySP` **V1.0.0** (was v0.0.1+20), `DaisyEffects` **a907b00e** (current
master), plus `DaisyInputs` **a7023845** added as a *new* submodule. All four were the latest
available upstream as of this date. Built clean on the first attempt and runs correctly on the
pedal. Full step-by-step recipe for repeating this on another project: see
"Library upgrade recipe" section below.

Two things learned the hard way during hardware testing, both documented in
`DEV_ENVIRONMENT_SETUP.md` step 9:
- **Press the Daisy Seed's RESET button before starting a debug session (F5).** Without NRST
  wired, J-Link can't reset-and-halt a target that's already running firmware — it fails with
  "Connecting to target failed. Connected correctly?", which looks like a wiring fault but
  isn't. A freshly-powered board attaches fine; a running one doesn't.
- **The board needs its own power** (USB/5V). The J-Link probe carries debug signals only and
  does not power the target.

Still on old libraries and candidates for the same recipe: **TheMartian** and
**TerrariumTemplate** (both `libdaisy` v5.3.0+11, `DaisySP` v0.0.1+29, old DaisyEffects/
DaisyInputs pins — note both already have a `lib/DaisyInputs` submodule, just older, so their
upgrade should be less involved than DaisyMulti's), **DaisyTemplate** (libdaisy/DaisySP pinned
to branch tips, no DaisyEffects/DaisyInputs), and **TheCommander** — whose submodule setup is
*broken*: `git submodule status` fails with
`not a git repository: lib/DaisyEffects/..\..\.git\modules\DaisyEffects`, so its `.git/modules`
wiring needs repairing before any bump. TheFirmament was already current.

**2026-07-02 — Dev environment fully reconstructed after Windows reinstall, verified working.**
Full walkthrough lives at [.claude/DEV_ENVIRONMENT_SETUP.md](DEV_ENVIRONMENT_SETUP.md)
(includes the corrected, current toolchain download link — the old
`github.com/electro-smith/DaisyToolchain` repo is stale/dead since 2022; the real
current installer is CDN-hosted and linked from `docs.daisy.audio`). Confirmed working
end-to-end on TheFirmament: build + flash + J-Link debug via F5. Two machine-specific
gotchas hit and fixed during setup, worth knowing before repeating this on DaisyMulti
or any other project folder:
- **`make` must run in a Git Bash terminal**, not PowerShell/cmd — Make's recipes
  (`rm -fR build`) are POSIX shell commands and PowerShell has no `sh.exe` for Make
  to shell out through. Switch VS Code's terminal profile to Git Bash.
- **J-Link installs into a version-numbered folder** (e.g.
  `C:\Program Files\SEGGER\JLink_V956\`, not a stable unversioned `...\JLink\` path).
  TheFirmament's `.vscode/launch.json` `serverpath` had to be updated to point at the
  actual versioned folder — this will need re-updating again after any future J-Link
  software update bumps the version number.
- TheFirmament's `.vscode/launch.json` also still had a stale `C:/Workspace/...`
  `JLinkScriptFile` path left over from before the reinstall (workspace is now on
  `D:`) — fixed. Worth grepping other project `.vscode` folders for the same
  leftover `C:/Workspace` path if debug launches fail there too (checked
  DaisyMulti/TheCommander/TheMartian/TerrariumTemplate/DaisyTemplate at the time —
  none had it, but re-check if untouched since the reinstall).

**Next up: the WorshipPad port into DaisyMulti** (see "Porting plan" below). Its blocker —
bringing DaisyMulti's vendored libraries up to date — is now cleared, and the upgrade
changed the port's starting position: `WorshipPad.cpp/h` is *already present* in
`DaisyMulti/lib/DaisyEffects/`, since it's part of the DaisyEffects master that the
submodule now points at. No file copying needed; the work is now registration + adapting
`Process()` to the sequential chain.

## Workspace overview

Multiple guitar-pedal firmware projects built on the Electrosmith Daisy Seed
(STM32H750), using `libdaisy` + `DaisySP`/`DaisySP-LGPL`, built with the
DaisyToolchain (`arm-none-eabi-gcc`, `make`). Toolchain lives at
`C:\Program Files\DaisyToolchain`. Debugging via SEGGER J-Link
(`JLinkGDBServerCL.exe`, currently version `JLink_V956` on this machine).

Active current goal: **port the "Worship Pad" effect from TheFirmament into
DaisyMulti**, the multi-effects pedal, as a new selectable effect slot — gated on
first bringing DaisyMulti's vendored libraries up to date (see Status log above).

---

## TheFirmament (source of the Worship Pad effect)

Path: `D:\Workspace\Daisy\TheFirmament`
PCB: Terrarium (custom pedal PCB + Daisy Seed). Originated from `TerrariumTemplate`,
renamed early in development (commit `f1946fa`).

### Purpose
Single-purpose pedal: a continuously-sustaining ambient/worship pad (root + fifth
oscillators through a long reverb) mixed in parallel with a clean-boosted dry
guitar signal. Designed for live worship/ambient guitar use — pad drones under
whatever the guitarist plays, with chromatic key switching via footswitches.

### Structure
- `src/TheFirmament.cpp` / `.h` — entry point + audio callback
- `include/PedalConfig.h` — pin mapping, DEV_BOARD vs production board, debug/display flags
- `lib/DaisyEffects/WorshipPad.cpp/h` — the pad effect itself
- `lib/DaisyEffects/CleanBoost.cpp/h` — always-on guitar boost stage
- `lib/DaisyEffects/IEffect.h` — base effect interface (see below)
- `lib/DaisyEffects/Hardware/DaisyDisplay.cpp/h` — 128x64 SSD1309 OLED driver
- `lib/DaisyEffects/Hardware/Storage.h` — `EffectSettings` struct (4 knobs + toggle position)
- `lib/DaisyInputs/` — `Button.h`, `Knob.h`, `NFNToggle.h`, `SimpleToggle.h` (debounce/jitter-filtered input wrappers)
- `lib/DaisyEffects/` also contains ~20 unused skeleton effect implementations left over from the template

### Audio callback (parallel mix, NOT a chain)
```cpp
float pad = worshipPad.Process(0.0f);          // pad ignores its input entirely
float boostedGuitar = boost.Process(in[0][i]); // guitar path
out[0][i] = (boostedGuitar + pad) * 0.7f;      // parallel mix + clip-guard scaling
```
No bypass logic — pad is always on (an on/off toggle existed briefly but was
removed in commit `d513768`; team decided it should just always run).

### WorshipPad DSP details
- Two `daisysp::Oscillator` sine generators: `oscRoot` and `oscFifth` (fifth =
  root × 1.49831, i.e. +7 semitones), each at 0.3f amplitude, summed to mono.
- Fed into `daisysp::ReverbSc` (LGPL module) placed in SDRAM via `DSY_SDRAM_BSS`
  (~386 KB buffer). Feedback = 0.90 (long tail), LP cutoff = 6000 Hz.
- **ReverbSc runs poorly at 96 kHz in real time** — Firmament works around this
  by processing the reverb at `sr/2`. Check whether DaisyMulti's existing
  `Reverb.cpp` (which also wraps `ReverbSc` at 96 kHz) already handles this the
  same way — reuse that pattern rather than re-deriving it.
- Chromatic root-note selection (12 semitones, C3/C4/C5 octave via a 3-position
  toggle), frequencies recalculated on key change (`UpdateFrequencies()`).
  Special-cased: G through B get halved (×0.5) to keep the octave toggle's
  physical position musically sensible.
- Only **1 of 4 knobs** is currently wired: `padLevel` (0.0–1.0, output gain on
  the wet reverb signal). Knobs 2–4 are unused headroom.
- Key switching is via two dedicated GPIO footswitches (`keyUpButton` /
  `keyDownButton`, rising-edge detected), not knobs — Firmament has spare
  buttons that DaisyMulti does not.
- OLED shows the current root note name (large scaled font), nothing else —
  display is reserved for key display, not knob/parameter values.

### Git history shape (49 commits, May 8 – Jun 24 2026)
Rough phases: (1) TerrariumTemplate boilerplate → renamed to TheFirmament, (2)
first WorshipPad added (root+fifth osc, replacing a Bypass placeholder), (3)
guitar+pad parallel mixing introduced + clipping-guard scaling, (4) key-switch
buttons + LED indicator added, (5) OLED key display added, (6) octave/frequency
tuning fix for G–B, (7) removed the pad on/off toggle (made it always-on) and
null-guarded display macro calls. Net effect: the design intentionally
converged on "always-on drone, footswitch-controlled key, minimal UI."

---

## DaisyMulti (target pedal for integration)

Path: `D:\Workspace\Daisy\DaisyMulti`
Hardware: Daisy Seed, 96 kHz sample rate, block size 1 (streaming/low-latency).
138 commits, actively developed (primary contributor tag `samiam21`, ticket
prefix `DP-`), currently mid-work on a drum-synthesis "StompBox" effect.

### Structure (as of the 2026-09-15 library upgrade)
- `src/DaisyMulti.cpp/h` — main state machine + audio callback
- `lib/DaisyEffects/` — one file pair per effect, all implementing `IEffect`
- `lib/DaisyEffects/Hardware/DaisyDisplay.cpp/h` — 128x64 SSD1309 OLED
- `lib/DaisyEffects/Hardware/Storage.h` — flash-persisted `EffectSettings`
- `lib/DaisyInputs/` — `Knob.h`, `Button.h`, `NFNToggle.h`, `SimpleToggle.h`
- `lib/Helpers/TempoArray.h` — tap-tempo averaging (DaisyMulti-specific, not in any library)
- `include/PedalConfig.h` — pins/config; `include/Effects.h` — effect enum +
  factory (`GetEffectObject`) + reverse lookup (`GetEffectType`)

**Changed in the upgrade** — the old layout had `src/DaisyDisplay.cpp/h`, `include/Storage.h`
and `lib/Inputs/` as project-local files. All three are gone: DaisyDisplay and Storage now
come from `lib/DaisyEffects/Hardware/` (DaisyEffects bundles them), and `lib/Inputs/` was
replaced by the `lib/DaisyInputs` submodule. Keeping local copies alongside the library ones
breaks the build — see the recipe section for why.

`include/PedalConfig.h` carries a `#define DEV_BOARD` toggle selecting between two completely
different pin maps (dev board vs production board). As of 2026-09-15 it is commented out, i.e.
building for the **production** board — check its current state before reasoning about pins.

### IEffect interface (nearly identical to Firmament's — same lineage)
```cpp
class IEffect {
  virtual void Setup(daisy::DaisySeed*, DaisyDisplay*, int *newBpm = nullptr) {}
  virtual void Cleanup() {}
  virtual float Process(float in) { return 0; }
  virtual void Loop(bool allowEffectControl, bool isTapPressed = false) {}
  virtual char *GetEffectName() { return (char*)"uh-oh"; }
  virtual char **GetKnobNames() { return (char**)""; }
  virtual EffectSettings GetEffectSettings() { return effectSettings; }
  virtual void SetEffectSettings(EffectSettings) {}
  virtual void UpdateToggleDisplay() {}
};
```
Because this matches Firmament's `IEffect` almost exactly, `WorshipPad.cpp/h`
should drop into `lib/DaisyEffects/` with only moderate changes (see "Porting
plan" below) — no interface redesign needed.

### Effect chain model — IMPORTANT difference from Firmament
DaisyMulti has **6 effect slots**, each holding one `IEffect*`, processed
**sequentially** (not parallel-mixed like Firmament):
```cpp
for (int j = 0; j < MAX_EFFECTS; j++)
  if (currentEffectsState[j]) wet = currentEffects[j]->Process(wet);
```
Each slot is independently toggleable on/off via one of 6 footswitches.
Firmament's WorshipPad *ignores* its `in` parameter and just returns the pad
signal — dropped into this chain unmodified, it would **replace** whatever
signal reached that slot instead of layering under it. **Decision needed:**
WorshipPad's `Process()` should probably become `return in + wetPad;` (mix
with the incoming chain signal) rather than ignoring `in`, so it behaves as a
"pad layer" slot like Firmament intended, composable with other effects
before/after it in the chain.

### Existing effects (17, in `lib/DaisyEffects/`)
CleanBoost, DaisyChorus, DaisyFlanger, DaisyTremolo, DaisyPhaser, Shifter
(PitchShifter), AutoWah, DaisyFold, Crush (BitCrush), Drive (Overdrive),
Distortion, Echo (DelayLine in SDRAM, tap-tempo synced), **Reverb (ReverbSc,
SDRAM)** — check this one first when porting WorshipPad's reverb handling,
DaisyResonator, Metronome, DaisyCompressor, StompBox (in-progress drum synth).

### UI / controls
- 4 knobs (pots), 6 footswitches (effect on/off, or slot-select in Edit Mode),
  1 encoder (press = toggle Play/Edit mode; turn = output volume in Play mode,
  cycle through the 17 effect types in Edit mode for the selected slot).
- No spare dedicated buttons like Firmament's key-up/down — **root-note
  selection for the ported pad will need to happen via a knob** (quantized to
  12 semitones) instead of footswitches, since all 6 footswitches are already
  committed to effect on/off.
- OLED: Play Mode shows all 6 active effect names + output level; Edit Mode
  shows the selected effect's name + 4 knob labels/values.
- Flash (QSPI) persistence of each slot's `EffectSettings` (4 knobs + toggle)
  already exists — no new storage code needed for a ported effect.

### Build/constraints
- `-Os`, all `.cpp` under `lib/DaisyEffects/` auto-included by the Makefile —
  adding a new effect needs no Makefile changes.
- SDRAM already hosts Echo's delay line and Reverb's `ReverbSc` buffer;
  Daisy Seed has substantial SDRAM (the subagent's "~512 KB total" estimate
  looked low/uncertain — verify actual SDRAM size before treating this as a
  hard constraint) — check headroom before adding a third large SDRAM
  consumer (WorshipPad's reverb).

---

## Library upgrade recipe (proven on DaisyMulti 2026-09-15)

For any pedal project still on the old local-`lib/Inputs` / local-`Storage.h` /
local-`DaisyDisplay.h` layout (TheMartian, TerrariumTemplate, TheCommander):

1. **Bump submodules.** `lib/libdaisy` → latest tag, `lib/DaisySP` → latest tag, then
   `git submodule update --init --recursive` (pulls DaisySP's nested `DaisySP-LGPL`),
   `lib/DaisyEffects` → latest master. **Also run `git submodule update --init --recursive`
   *inside* `lib/libdaisy`** — its own HAL/CMSIS/USB nested submodules aren't fetched by a
   top-level update, and the build otherwise dies with
   `stm32h7xx_hal.h: No such file or directory`.
2. **Add `lib/DaisyInputs`** (`https://github.com/samiam21/DaisyInputs`, branch `main`) and
   **delete local `lib/Inputs/`**. Keeping both duplicate-links, because current DaisyEffects
   headers `#include "../DaisyInputs/..."` rather than `"../Inputs/..."`.
3. **Delete local `src/DaisyDisplay.h/.cpp` and `include/Storage.h`**; repoint the project's
   own includes at `../lib/DaisyEffects/Hardware/DaisyDisplay.h`. `IEffect.h` now pulls
   `Hardware/DaisyDisplay.h` internally, so a project keeping its own DaisyDisplay class ends
   up with a *different type of the same name* than the one `IEffect::Setup()` expects — a
   confusing compile error if you miss this.
4. **Makefile:** swap the `lib/Inputs` wildcard for `lib/DaisyInputs`, add
   `lib/DaisyEffects/Hardware` to `CPP_SOURCES` (and drop the explicit `src/DaisyDisplay.cpp`
   entry), then add the LGPL wiring — needed by `DaisyCompressor`/`DaisyFold`/reverb effects,
   since DaisySP v1.0.0 moved `Compressor`/`ReverbSc`/`Fold` into the LGPL sub-library:
   ```make
   DAISYSP_LGPL_DIR = lib/DaisySP/DaisySP-LGPL
   C_INCLUDES += -I$(DAISYSP_LGPL_DIR)/Source -DUSE_DAISYSP_LGPL
   LIBS       += -ldaisysp-lgpl
   LIBDIR     += -L$(DAISYSP_LGPL_DIR)/build
   ```
   (`C_INCLUDES` is used rather than `CPPFLAGS` because the core Makefile does
   `CPPFLAGS = $(CFLAGS)`, which would clobber it; `C_INCLUDES ?=` respects a pre-set value.)
5. **`include/PedalConfig.h`:** add `#define KNOB_NO_CHN 99` and
   `const int effectTogglePin1/2 = effectSPSTPins[0]/[1];` in *each* board variant block.
   Updated effect classes reference these in default member initializers, so they must exist
   even if that effect's default config never uses them.
6. **Effect factories need no changes.** Per-effect classes gained optional configuration
   methods (`ConfigureKnobPositions`, `SetAlwaysOn`, …) but stayed default-constructible and
   `IEffect`-compatible, so a `new EffectClass()` factory like `include/Effects.h` still compiles
   untouched.
7. Build order: `make` in `lib/libdaisy`, then `lib/DaisySP` (its default `all` target builds
   the LGPL sub-library too), then the project. Remember Git Bash, not PowerShell.

---

## Porting plan: WorshipPad → DaisyMulti

1. ~~Copy `WorshipPad.cpp/h` into `DaisyMulti/lib/DaisyEffects/`~~ — **already done for you**
   by the 2026-09-15 library upgrade: the files ship with the DaisyEffects master the submodule
   now tracks, and their includes already resolve (`IEffect.h`, `Hardware/Storage.h`,
   `PedalConfig.h`). They are compiled into the build today via the Makefile's
   `lib/DaisyEffects/*.cpp` wildcard — WorshipPad is simply not *registered* as a selectable
   effect yet. Start at step 2.
2. **Change `Process()` to mix with `in`** instead of ignoring it (see above),
   so it composes with the sequential effect chain.
3. Reuse DaisyMulti's existing `Reverb.cpp` sample-rate workaround for
   `ReverbSc` at 96 kHz (Firmament used `sr/2`) rather than re-deriving it —
   check that file first.
4. Re-map controls: pad level stays knob 1; use knob 2–4 for root note
   (quantized), reverb decay/feedback, and tone (LP cutoff) — Firmament only
   wired 1 of 4 knobs, so there's headroom to expose more here.
5. Root note selection moves from Firmament's dedicated footswitches to a
   quantized knob, since DaisyMulti's footswitches are all committed to
   effect on/off toggling. Confirm this UX tradeoff with the user before
   finalizing — it changes how playable/performable key-switching feels
   live.
6. Register in `include/Effects.h`: add enum value, `GetEffectObject()` case,
   `GetEffectType()` reverse-lookup case. No Makefile changes needed.
7. Decide the on/off behavior: DaisyMulti's slot model gives WorshipPad a
   real bypass toggle for free (Firmament deliberately removed its own
   toggle and made the pad always-on) — worth deciding whether the ported
   version should default differently.
8. Build, flash, and test in Edit Mode (cycle encoder to find it in the
   17→18 effect list) and Play Mode (toggle on/off, verify it mixes under
   other active effects in the chain rather than replacing them).
