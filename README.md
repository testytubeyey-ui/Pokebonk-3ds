# PokéBonk 2DS Mini Recreation

A tiny original, PokéBonk-inspired homebrew prototype for Nintendo 3DS / New 2DS XL.
This is not the original PokéBonk game and does not include its copyrighted assets.

## Current prototype
- D-pad movement
- A button attacks nearby enemies
- Enemies slowly chase the player
- XP, levels, health, and a score
- Console-rendered arena (placeholder graphics)

## Build
You need a computer with devkitPro and the 3DS development tools installed.
Install the 3DS toolchain (`3ds-dev`) and libctru using the devkitPro installer.

From this project folder, run:

    make

This produces `pokebonk_2ds_mini.3dsx`, which can be launched using Homebrew Launcher
on a homebrew-enabled 3DS/2DS.

## CIA
This starter project does not yet produce a CIA. CIA packaging needs additional
3DS metadata and tools, and should be tested on the actual hardware. Do not install
a CIA from an unknown source.

## Controls
- D-pad: move
- A: bonk nearby enemies
- START: quit
