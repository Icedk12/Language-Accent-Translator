# Language-Accent-Translator
## Bugs
It would be easy to fix, but if you type something that is not an option in LAT-gui it breaks. Ngl ima take a nap lmk.

## Overview
A tool designed to bridge the gap between different coding styles and dialects, allowing developers to seamlessly translate their preferred shorthand or personal code style ("accent") into standard target syntax. It can either be used as an interface or pure commands. LAT can be used to draft different syntax styles in the development of programming languages too, giving the user the ability to try out their syntax on an already existing language.

**NOTE:** LAT is designed to be used prior to the compilation process and may cause syntax warnings before being translated to a standard accent.

**LAT** uses the .lat file extension to automatically find and replace keywords with others. An example of the syntax can be found below.
```
list = std::vector
use = #include
i64 = int
string = std::string
```

## Command List:
These commands only apply to LAT.exe and not LAT-gui.exe.
```
  lat accent <path>      Set the accent rules file.
  lat file <path>        Set the source file to translate.
  lat translate -m       Translate into your accent.
  lat translate -o       Translate into the other accent.
  lat status             Show current file settings.
  lat cls                Clear the console. (deprecated, just use cls)
  lat help               Show this help.
```
# Appendix
## Credits and License
Tom Patton-Low

## Notes
I realised writing this that LAT could be for purposes like translating real languages too, so uh that's a thing.
