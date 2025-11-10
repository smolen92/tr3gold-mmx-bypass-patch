# Tomb Raider 3 Lost Artefact/Artifact MMX bypass patch

## Overview
This patch for Tomb Raider 3 Lost Artefact fixing issue when the game freeze and crash before loading icons in the main menu.

## Problem Description
The game is using MMX instruction that are not supported (not sure if this is windows 10 problem or modern processors don't support these instruction anymore)<br>
The game has fortunatelly option to disable this in config file, however this option isn't displayed in settings window.<br>
Unfortunatelly by default the game will turn this feature on and because of it will crash and no config file will be created.

This patch change that the default value is 0, turning this settings off.

## Compilation 

1. Clone this repository
2. Install OpenSSL
3. run make
for windows:
```bash
make tr3gold-mmx-bypass.exe
```
for linux:
```bash
make
```

## Usage:
Run the patch with:
```bash
tr3gold-mmx-bypass.exe -i=<path-to-the-executable>
```
Example:
```bash
tr3gold-mmx-bypass.exe "C:\Games\Eidos\Tomb Raider 3 - The Lost Artefact\tr3gold.exe"
```

if no input file is specified, the program will try to open tr3gold.exe<br>
the program can be run in terminal/command prompt with --cli argument
```bash
tr3gold-mmx-bypass.exe --cli
```

### Notes
- This patch will backup the original executable to a new file with a .bak extension.
E.g: tr3gold.exe will be backup to tr3gold.bak
- If you get error that permission is denied, check if the executable doesn't have the read-only attribute set.
- It is possible to edit config file and turn MMX settings on, if this is changed the game will crash at the main menu again.

## Compatability

### Game Release Version
- This patch is compatible with version from the Tomb Raider Ultimate Edition that was released for Czech/Slovak/Polish market.
Other version has not been tested and therefore compatabily with other versions are yet unknown.

### Windows
- This patch have been tested on windows 10 on real machine and in VM, in both cases t was able to start and get into the game.

### Linux through Proton
- When runnig the game on linux with proton the game, the game need to be installed outside Program Files or Program Files (x86) folders.
If the game in installed in these folders the game cannot find the file TITLE.TR2, this could be only issue with the Ultimate Edition release.
The file is in cd directory and in this version is patched so the game doesn't require cd in drive. On original release the file is probaly on CD.

## Todo
- check if the game can be beat with this patch
- check the patch on other releases of the game
