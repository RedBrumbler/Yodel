# Yodel
A (WIP) modloader specific to echovr for quest (possibly PC at some point?) inspired by [scotland2](https://github.com/sc2ad/scotland2)

# Features
 - loading stuff
 - app init somewhere after all the modules are loaded (all the keyboardsmash libs)?
 - yelling at code

# Installation/Usage

This is a [Libr15Loader](https://github.com/RedBrumbler/Libr15Loader) compatible modloader. Thus it should be placed at: `/sdcard/ModData/<app ID>/Modloader/libyodel.so`.
This modloader has several call in points, and it performs a topological sort of all dependents that need to be loaded and loads them in turn.

| | Libs | Early Mods | Mods |
|-|------|------------|------|
| app start | `dlopen` | `dlopen`, `setup` | not initialized yet |
| r15 load | nothing called | `load` | not initialized yet |
| app init | nothing called | `late_load` | `dlopen`, `setup`, `late_load` |

# Resources
 - Inspired by [scotland2](https://github.com/sc2ad/scotland2)
 - Easier time patching with [EchoPatcher](https://github.com/Lauriethefish/EchoPatcher) by @Lauriethefish
