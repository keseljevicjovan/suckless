# Suckless repo

This repository is used for personal builds of suckless software. It includes:

- dynamic window manager ([suckless link](https://dwm.suckless.org)) 
- simple terminal ([suckless link](https://st.suckless.org))
- dmenu ([suckless link](https://tools.suckless.org/dmenu))
- dwmblocks-async ([github link](https://github.com/UtkarshVerma/dwmblocks-async))
- simple lock ([suckless link](https://tools.suckless.org/slock))

## Installation

Clone the repository and build all software:

```
git clone https://github.com/keseljevicjovan/suckless
cd suckless
for i in ./*; do ( cd $i; sudo make install clean; cd .. ); done
```

## Patches

Also available in the patches directories:

#### dwm
 - actualfullscreen 
 - alpha
 - alwayscenter
 - fullgaps
 - fullscreen
 - movecenter
 - nomonocleborders
 - push_no_master
 - scratchpad
 - swallow
 - xresources

#### st
 - alpha
 - boxdraw
 - columnredraw
 - scrollback
 - scrollback-mouse
 - xresources

#### dmenu
 - alpha
 - border
 - center
 - grid
 - xresources

#### dwmblocks-async 
  - [ No patches ]

#### slock
 - dwmlogo


## Compatibily with XResources

dwm, st, and dmenu support XResources. For color schemes, scripts and configuration, see my [dotfiles](https://github.com/keseljevicjovan/dotfiles). This allows easy changes without recompiling.


## FAQ

#### Q: How do I change keybindings/colors/fonts ?
A: Open config.h (not config.def.h), edit it, compile with ```sudo make install clean``` and then restart program. 

#### Q: My status bar is not showing any icons ?
A: Because it's not included directly in dwmblocks. You need to get status bar scripts (here is [link](https://github.com/keseljevicjovan/dotfiles/tree/main/.local/bin/statusbar) for mine), and then put location to them in $PATH. 

#### Q: Why suckless software ?
A: Because it [sucks less](https://suckless.org/philosophy/).

#### Q: I use login manager, how do I start dwm?
A: Make dwm.desktop entry, put it in /usr/share/xsessions/, and select it in sessions.
