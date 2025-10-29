# Suckless repo

This repository is used for personal builds of suckless software. It includes:

- dynamic window manager ([suckless.org](https://dwm.suckless.org)) 
- simple terminal ([suckless.org](https://st.suckless.org))
- dmenu ([suckless.org](https://tools.suckless.org/dmenu))
- dwmblocks-async ([github.com](https://github.com/UtkarshVerma/dwmblocks-async))
- slock ([suckless.org](https://tools.suckless.org/slock))
- nsxiv ([github.com](https://github.com/nsxiv/nsxiv)) ([codeberg.org](https://codeberg.org/nsxiv/nsxiv))

## Installation

Clone the repository and build all software:

```sh
git clone https://github.com/keseljevicjovan/suckless
sed -i "s|jovan|$(whoami)|g" suckless/slock/config.h
for i in ./suckless/*; do ( cd $i; sudo make install clean; cd .. ); done
```

## Patches

Also available in the patches directories:

#### dwm (13)
 - [actualfullscreen](https://dwm.suckless.org/patches/actualfullscreen/) 
 - [alpha](https://dwm.suckless.org/patches/alpha/)
 - [alwayscenter](https://dwm.suckless.org/patches/alwayscenter/)
 - [fullgaps](https://dwm.suckless.org/patches/fullgaps/)
 - [fullscreen](https://dwm.suckless.org/patches/fullscreen/)
 - [movecenter](https://dwm.suckless.org/patches/movecenter/)
 - [nomonocleborders](https://github.com/keseljevicjovan/suckless/blob/main/dwm/patches/dwm-nomonocleborders-6.5.diff)
 - [push_no_master](https://dwm.suckless.org/patches/push/)
 - ~~[restoreafterrestart](https://dwm.suckless.org/patches/restoreafterrestart/)~~
 - [scratchpad](https://dwm.suckless.org/patches/scratchpad/)
 - [setborderpx](https://dwm.suckless.org/patches/setborderpx/)
 - [statusallmons](https://dwm.suckless.org/patches/statusallmons/)
 - [swallow](https://dwm.suckless.org/patches/swallow/)
 - [xrdb](https://dwm.suckless.org/patches/xrdb/)
 - ~~[xresources](https://dwm.suckless.org/patches/xresources/)~~

#### st (3)
 - [alpha](https://st.suckless.org/patches/alpha)
 - [boxdraw](https://st.suckless.org/patches/boxdraw)
 - ~~[columnredraw](https://github.com/abhaysp95/st_custom/blob/master/patches/st-columnredraw-20210722-e40efda.diff)~~
 - ~~[scrollback](https://st.suckless.org/patches/scrollback)~~
 - ~~[scrollback-mouse](https://st.suckless.org/patches/scrollback)~~
 - ~~[xst-xresources](https://github.com/gavales/st/blob/master/diffs/xst-xresources-20200626.diff)~~
 - [xresources-with-reload-signal](https://st.suckless.org/patches/xresources-with-reload-signal)
 - ~~[xresources](https://st.suckless.org/patches/xresources)~~

#### dmenu (7)
 - [alpha](https://tools.suckless.org/dmenu/patches/alpha)
 - [border](https://tools.suckless.org/dmenu/patches/border)
 - [center](https://tools.suckless.org/dmenu/patches/center)
 - [grid](https://tools.suckless.org/dmenu/patches/grid)
 - ~~[xresources](https://tools.suckless.org/dmenu/patches/xresources)~~
 - [xresources-alt](https://tools.suckless.org/dmenu/patches/xresources-alt)
 - [vi-mode](https://tools.suckless.org/dmenu/patches/vi-mode)
 - [fuzzymatch](https://tools.suckless.org/dmenu/patches/fuzzymatch)

#### dwmblocks-async (0)
  - [ No patches ]

#### slock (1)
 - ~~[dwmlogo](https://tools.suckless.org/slock/patches/dwmlogo)~~
 - [background-image](https://tools.suckless.org/slock/patches/background-image)

#### nsxiv (2)
 - [alpha](https://github.com/nsxiv/nsxiv-extra/tree/master/patches/alpha)
 - [dmenu-search](https://github.com/nsxiv/nsxiv-extra/tree/master/patches/dmenu-search)

## Compatibily with XResources

dwm, st, dmenu and nsxiv support XResources. For color schemes, scripts and configuration, see my [dotfiles](https://github.com/keseljevicjovan/dotfiles). This allows easy changes without recompiling.

## FAQ

#### Q: How do I change keybindings/colors/fonts?
A: Open config.h (not config.def.h), edit it, then compile with 
```sh 
sudo make install clean
```
And restart the program to apply the changes.

#### Q: My status bar is not showing any modules?
A: The modules aren’t included directly in dwmblocks. You need to download status bar scripts (here is [link](https://github.com/keseljevicjovan/dotfiles/tree/main/.local/bin/statusbar) for mine) and add their location to your $PATH.

#### Q: Why suckless software?
A: Because it [sucks less](https://suckless.org/philosophy/).

#### Q: I use login manager, how do I start dwm?
A: Create a dwm.desktop file and place it in /usr/local/share/xsessions/. Then select dwm from your login session options.

Here is an example of the dwm.desktop file:
```desktop
[Desktop Entry]
Name=dwm
Comment=Dynamic Window Manager
Exec=/home/YOUR_USERNAME/.config/X11/xinit/dwm/xinitrc
Type=XSession
```

#### Q: How do I troubleshoot build errors?
A: Make sure you have all necessary packages installed for your system.

For Arch-based distros, use pacman:
```sh
sudo pacman -Syu base-devel libx11 libxft libxinerama libxrandr libxext libxrender fontconfig freetype2 libexif imlib2 shadow
```
