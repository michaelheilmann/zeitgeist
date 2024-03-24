# Zeitgeist
*Zeitgeist* is an interpreter to preserve my software creations.
It protects the actual software from environment idiosyncrasies like compilers and operating systems.

## Building Zeitgeist
*Zeitgeist* successfully builds and runs under *Windows 11* and *Linux*.

- [Building Zeitgeist: Windows 11/Visual Studio Community](building-under-windows-11-visual-studio-community-20222)
- [Building Zeitgeist: Linux](building-under-linux)

## Contribute
If you have questions on using *Zeitgeist* or you want to contribute,
contact me under [contact@primordialmachine.com](contact@priordialmachine.com).

# How Zeitgeist Works (2024)
The *Zeitgeist* interpreter is invoked via command line and expects the name of a *rendition* as its single parameter.
```
./Zeitgeist-Interpreter --rendition "Hello World"
```
*Renditions* are basically the programs the *Zeitgeist* interpreter executes.
Here, we specified that the rendition of the name *Rendition 1* should be executed.
If the rendition can be loaded, the *Zeitgeist* interpreter will execute that rendition and terminate.

This particular rendition is always shipped with *Zeitgeist*, it simply prints `Hello, World!` to standard output. 

Under windows, the available renditions reside in the directory *Renditions* inside the folder where the *zeitgeist.exe* executable resides in.
You can see the list of *renditions* shipped with *Zeitgeist* by finding the executalbeentering the directoy
Alternatively, you can invoke
```
./Zeitgeist-Interpreter --list-renditions
```
to list all renditions available to the *Zeitgeist* interpreter.

# Recommended Editors/IDEs when working on Zeitgeist's Code
The following tools are my personal recommendations only:
- Under Windows, I can recommend *Notepad++* ([https://notepad-plus-plus.org/](https://notepad-plus-plus.org/)].
- Under Linux, I am using *Notepadqq* as a replacement for *Notepad++*.

I am using either the *DejaVu Sans Mono* font ([https://dejavu-fonts.github.io](https://dejavu-fonts.github.io) with a font size of 9
or the *Hack* font ([https://github.com/source-foundry/Hack](https://github.com/source-foundry/Hack).
