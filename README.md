# Suckless repo (Wayland)

**This branch only includes Wayland suckless utilities. For X11 utilities checkout the 'main' branch.**

This repository is used for personal builds of suckless software. It includes:

- dwl ([codeberg.org](https://codeberg.org/dwl/dwl)) 
- dwlb ([github.com](https://github.com/kolunmi/dwlb))
- dwlmsg ([codeberg.org](https://codeberg.org/notchoc/dwlmsg))
- mew ([codeberg.org](https://codeberg.org/sewn/mew)) 
- someblocks ([sourcehut.hub](https://git.sr.ht/~raphi/someblocks))

## Installation

Clone the repository and build all software:

```sh
git clone -b wayland --single-branch https://github.com/keseljevicjovan/suckless
for i in ./suckless/*; do ( cd $i; sudo make install clean; cd .. ); done
```

## Patches

Also available in the patches directories:

#### dwl (5)
 - [alwayscenter](https://dwm.suckless.org/patches/alwayscenter/)
 - [gaps](https://dwm.suckless.org/patches/fullgaps/)
 - [ipc](https://dwm.suckless.org/patches/fullscreen/)
 - [kblayout](https://dwm.suckless.org/patches/xrdb/)
 - [swallow](https://dwm.suckless.org/patches/xresources/)

## FAQ

#### Q: How do I change keybindings/colors/fonts?
A: Open config.h (not config.def.h), edit it, then compile with 
```sh 
sudo make install clean
```
And restart the program to apply the changes.

#### Q: My status bar is not showing any modules?
A: The modules aren’t included directly in someblocks. You need to download status bar scripts (here is [link](https://github.com/keseljevicjovan/dotfiles/tree/main/.local/bin/statusbar) for mine) and add their location to your $PATH.

#### Q: Why suckless software?
A: Because it [sucks less](https://suckless.org/philosophy/).

#### Q: Why Wayland instead of X11?
A: Mainly because of better multi-monitor handling and a bit improved security model. Both X11 and Wayland branches will still be maintained and updated, so it’s not a hard switch.

#### Q: I use login manager, how do I start dwl?
A: Copy dwl/dwl.desktop and place it in /usr/local/share/wayland-sessions/. Then select dwl from your login session options.

#### Q: How do I troubleshoot build errors?
A: Make sure you have all necessary packages installed for your system.

#### Q: How do I make my dwl status bar transparent?
A: Change the color values. They are in RGBA format, last two hex digits are alpha.
