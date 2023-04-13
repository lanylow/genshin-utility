# Genshin Utility [![License](https://img.shields.io/badge/License-GPL3.0-green.svg)](https://github.com/lanylow/genshin-utility/blob/main/LICENSE) ![OS](https://img.shields.io/badge/OS-Windows-green.svg)

Free and open-source tool for Genshin Impact that includes a few quality-of-life features, developed as an internal module that's being loaded into the game process. Compatible only with the global version of the game.

## FAQ

### How to load the mod?
1. Download and extract binaries from the [latest release](https://github.com/lanylow/genshin-utility/releases).
2. Open `loader.exe` **as administrator**, a window should pop up informing you that it's waiting for the game.
3. Now with that running, open Genshin Impact.
4. The game will open and the mod will be loaded.

### How do I open and close the menu?
When you have the Genshin Impact window focused, press the <kbd>INSERT</kbd> key.

### Where is my configuration saved?
The `genshin-utility` folder within your Documents directory contains the saved configuration files. These files are in a human-readable format and can be edited as needed. However, after updates, it may be necessary to delete and recreate the configuration file.

## Screenshots

![screenshot](https://user-images.githubusercontent.com/31806776/199519142-020e5ded-3b42-46dd-bd40-846a2a9caea1.png)

## Utilized third-party libraries

* [imgui](https://github.com/ocornut/imgui) - Graphical User interface for C++
* [json](https://github.com/nlohmann/json) - JSON for Modern C++ 
* [kiero](https://github.com/Rebzzel/kiero) - Universal graphical hook
* [minhook](https://github.com/TsudaKageyu/minhook) - API Hooking Library for Windows 

## License

This project is licensed under the GPL-3.0 License - see the [LICENSE](https://github.com/lanylow/genshin-utility/blob/main/LICENSE) file for details.
