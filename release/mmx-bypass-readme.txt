Tomb Raider 3 Lost Artefact/Artifact MMX bypass patch

Overview:
This patch for Tomb Raider 3 Lost Artefact fixes an issue when the game freezes and crashes before loading icons in the main menu.

Usage:

tr3gold-mmx-bypass.exe -i=<path-to-the-executable>

Example:

tr3gold-mmx-bypass.exe -i="C:\Program Files (x86)\Eidos\Tomb Raider 3 - The Lost Artefact\tr3gold.exe"

If no input file is specified, the patch will try to open tr3gold.exe it the current directory
The program can be run in terminal/command prompt with the argument --cli

Example:
tr3gold-mmx-bypass.exe --cli

Notes
- This patch will backup the original executable to a new file with a .bak extension. E.g: tr3gold.exe will be backup to tr3gold.exe.bak
- It is possible to edit the config file and turn MMX settings on, if this is changed, the game will crash at the main menu again.

Windows:
- if the patch cannot open the game executable, check whether the read-only flag is set (right click on the game executable and click on properties)
- if the patch still cannot open the game executable, run the patch as an Administrator

Linux:
- if the patch cannot open the game executable, check whether it has write permission and if not add it with the command "chmod +w tr3gold.exe"

