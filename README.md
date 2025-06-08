# Suckless repo

This repository is used for personal builds of suckless software. It includes:

- dynamic window manager ([suckless link](https://dwm.suckless.org)) 
- simple terminal ([suckless link](https://st.suckless.org))
- dmenu ([suckless link](https://tools.suckless.org/dmenu))
- dwmblocks-async ([github link](https://github.com/UtkarshVerma/dwmblocks-async))
- slock ([suckless link](https://tools.suckless.org/slock))
- nsxiv ([github link](https://github.com/nsxiv/nsxiv)) ([codeberg link](https://codeberg.org/nsxiv/nsxiv))

## Installation

Clone the repository and build all software:

```
git clone https://github.com/keseljevicjovan/suckless
cd suckless
for i in ./*; do ( cd $i; sudo make install clean; cd .. ); done
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

#### dmenu (5)
 - [alpha](https://tools.suckless.org/dmenu/patches/alpha)
 - ~~[border](https://tools.suckless.org/dmenu/patches/border)~~
 - ~~[center](https://tools.suckless.org/dmenu/patches/center)~~
 - [grid](https://tools.suckless.org/dmenu/patches/grid)
 - ~~[xresources](https://tools.suckless.org/dmenu/patches/xresources)~~
 - [xresources-alt](https://tools.suckless.org/dmenu/patches/xresources-alt)
 - [vi-mode](https://tools.suckless.org/dmenu/patches/vi-mode)
 - [fuzzymatch](https://tools.suckless.org/dmenu/patches/fuzzymatch)

#### dwmblocks-async (0)
  - [ No patches ]

#### slock (1)
 - [dwmlogo](https://tools.suckless.org/slock/patches/dwmlogo)

#### nsxiv (1)
 - [alpha](https://github.com/nsxiv/nsxiv-extra/tree/master/patches/alpha)

## Compatibily with XResources

dwm, st, dmenu and nsxiv support XResources. For color schemes, scripts and configuration, see my [dotfiles](https://github.com/keseljevicjovan/dotfiles). This allows easy changes without recompiling.

## FAQ

#### Q: How do I change keybindings/colors/fonts ?
A: Open config.h (not config.def.h), edit it, compile with ```sudo make install clean``` and then restart program. 

#### Q: My status bar is not showing any modules ?
A: Because it's not included directly in dwmblocks. You need to get status bar scripts (here is [link](https://github.com/keseljevicjovan/dotfiles/tree/main/.local/bin/statusbar) for mine), and then put location to them in $PATH.

#### Q: Why suckless software ?
A: Because it [sucks less](https://suckless.org/philosophy/).

#### Q: I use login manager, how do I start dwm?
A: Make dwm.desktop entry, put it in /usr/share/xsessions/, and select it in sessions.
