# Tomb Raider 3 Lost Artefact/Artifact MMX bypass patch  
  
## Overview  
This patch for Tomb Raider 3 Lost Artefact fixes an issue when the game freezes and crashes before loading the icons in the main menu.  
  
## Problem Description  
The game uses MMX instruction that are not supported (not sure if this is windows 10 issue or if modern processors no longer support these instruction)<br>
Fortunatelly, the game has option to disable this in the config file, but this option is not displayed in settings window.<br>  
Unfortunatelly by default the game enables this feature, and because of this it crashes and no config file is created.  
  
This patch change the default value to 0, turning this settings off.  
  
## Compilation  
  
1. Clone this repository  
2. Install SDL3, SDL3_image and SDL3_ttf C libraries 
3. run make  
for windows:  
```bash  
make OUTFILE=tr3gold-mmx-bypass.exe PLATFORM=windows MODE=release
```  
for linux:  
```bash  
make MODE=release
```  
  
## Usage:  
Run the patch with:  
```bash  
tr3gold-mmx-bypass.exe -i=<path-to-the-executable>  
```  
Example:  
```bash  
tr3gold-mmx-bypass.exe -i="C:\Games\Eidos\Tomb Raider 3 - The Lost Artefact\tr3gold.exe"  
```  
  
if no input file is specified, the program will try to open tr3gold.exe in the current directory<br>  
the program can be run in a terminal/command prompt with the --cli argument  
```bash  
tr3gold-mmx-bypass.exe --cli  
```  
  
### Notes  
- This patch will backup the original executable to a new file with a .bak extension.  
E.g: tr3gold.exe will be backup up to tr3gold.bak    
- It is possible to edit the config file and turn MMX settings on, if this is changed, the game will crash at the main menu again.  

#### Windows
- if the patch cannot open the game executable, check whether the read-only flag is set(right click on the game executable and click on properties)
- if the patch still cannot open the game executable, run the patch as Administrator

#### Linux
- if the patch cannot open the game executable, check if it has write permission and if not add it with command 
```bash
chmod +w tr3gold.exe
```
  
## Compatability  
  
### Game Release Version  
- This patch is compatible with version from the Tomb Raider Ultimate Edition released for Czech/Slovak/Polish market.  
Other version has not been tested, so compatabily with the are unknown.  
  
### Windows  
- This patch have been tested on windows 10 on physical machine and in a VM, in both cases, it was able to start and reach the game. 
  
### Linux through Proton  
- When runnig the game on linux with proton, the game needs to be installed outside the Program Files or Program Files (x86) folders.  
If the game in installed in these folders the game cannot find the file TITLE.TR2, this could be only issue with the Ultimate Edition release.  
The file is in the CD directory and in this version it has been patched so the game does not require the CD in the drive. In the original release the file is probably on the CD.  
  
## Todo  
~~- check if the game can be beaten with this patch~~ this is confirmed, the game can be beaten 
- check the patch on other releases of the game

