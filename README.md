# Genshin Utility [![License](https://img.shields.io/badge/License-GPL3.0-green.svg)](https://github.com/lanylow/genshin-utility/blob/main/LICENSE) ![OS](https://img.shields.io/badge/OS-Windows-green.svg)

A simple program that's supposed to change the default field of view and remove the 60 FPS cap. It works by injecting a dll into the process and hooking the setter of [this property](https://docs.unity3d.com/ScriptReference/Camera-fieldOfView.html). FPS unlocking is done by overwriting memory of UnityPlayer.dll.

## Usage

1. Download and extract binaries from the [latest release](https://github.com/lanylow/genshin-utility/releases).
2. Open CMD as administrator and `cd` into the extracted folder.
3. Now when you're in the folder, run the following command. Replace the `<path>` with location of `GenshinImpact.exe`:
```
loader.exe <path>
```
4. Genshin impact will now be opened and the program will be injected, a console will pop up with every setting explained.

Here is a [video showing the program](https://www.youtube.com/watch?v=cvZf11vtuaw).

## Credits

* [lanylow](https://github.com/lanylow) - for coding the whole thing.
* [KKKKKKKKKKKKK](https://github.com/34736384) - for the original FPS unlocker.
