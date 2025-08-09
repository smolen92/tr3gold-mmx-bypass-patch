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
2. Install OpenSSL for windows
3. Compile the patch with:
```bash
gcc -o tr3gold-mmx-bypass.exe main.cpp -I"C:\Program Files\OpenSSL-Win64\include" -L"C:\Program Files\OpenSSL-Win64\lib\VC\x64\MT" -lcrypto
```
4. Run the patch with:
```bash
tr3gold-mmx-bypass.exe <path-to-the-executable>
```
Example:
```bash
tr3gold-mmx-bypass.exe "C:\Program Files (x86)\Eidos\Tomb Raider 3 - The Lost Artefact\tr3gold.exe"
```
If no argument is entered, the patch will try to find the executable in these location:<br>
the directory patch is executed<br>
C:\Program Files\Eidos\Tomb Raider 3 - The Lost Artefact\tr3gold.exe<br>
C:\Program Files (x86)\Eidos\Tomb Raider 3 - The Lost Artefact\tr3gold.exe

### Notes
- This patch will backup the original executable to a new file with a .bak extension.
E.g: tr3gold.exe will be backup to tr3gold.exe.bak
- If you get error that permission is denied, check if the executable doesn't have the read-only attribute set.
- It is possible to edit config file and turn MMX settings on, if this is changed the game will crash at the main menu again.


## Compatability
- This patch is compatible with version from the Tomb Raider Ultimate Edition that was released for Czech/Slovak/Polish market.
Other version has not been tested and therefore compatabily with other versions are yet unknown.
- This patch have been tested only in a VM on windows 10, where it was able to start and get into the game.
- When runnig the game on linux with proton the game still crash at the main menu

## Todo
- check if the game can be beat with this patch
- try to find why it still crash on linux when running it with proton
