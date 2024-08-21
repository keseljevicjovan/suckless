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

#### dwm (10)
 - actualfullscreen 
 - alpha
 - alwayscenter
 - fullgaps
 - fullscreen
 - movecenter
 - nomonocleborders
 - push_no_master
 - ~~restoreafterrestart~~
 - scratchpad
 - swallow
 - ~~xrdb~~
 - ~~xresources~~

#### st (5)
 - alpha
 - boxdraw
 - columnredraw
 - scrollback
 - scrollback-mouse
 - ~~[xst-xresources](https://github.com/gavales/st/blob/master/diffs/xst-xresources-20200626.diff)~~
 - ~~xresources~~

#### dmenu (4)
 - alpha
 - border
 - center
 - grid
 - ~~xresources~~

#### dwmblocks-async (0)
  - [ No patches ]

#### slock (1)
 - dwmlogo


## Compatibily with XResources (Removed)

~~dwm, st, and dmenu support XResources. For color schemes, scripts and configuration, see my [dotfiles](https://github.com/keseljevicjovan/dotfiles). This allows easy changes without recompiling.~~

However, relying on XResources led to some issues, such as dmenu failing to start if XResources were not loaded, or st displaying weird colors. After consideration, I've decided that it's better to have no feature than one that is poorly implemented. In future, I might develop a custom patch for live reload. 

If you're interested in the live reload feature, you can refer to the [last commit where it was available](https://github.com/keseljevicjovan/suckless/tree/b01a0c37e6922890815be440dd65473d947171d4) or checkout [live-reload branch](https://github.com/keseljevicjovan/suckless/tree/live-reload).

## FAQ

#### Q: How do I change keybindings/colors/fonts ?
A: Open config.h (not config.def.h), edit it, compile with ```sudo make install clean``` and then restart program. 

#### Q: My status bar is not showing any modules ?
A: Because it's not included directly in dwmblocks. You need to get status bar scripts (here is [link](https://github.com/keseljevicjovan/dotfiles/tree/main/.local/bin/statusbar) for mine), and then put location to them in $PATH.

#### Q: Why suckless software ?
A: Because it [sucks less](https://suckless.org/philosophy/).

#### Q: I use login manager, how do I start dwm?
A: Make dwm.desktop entry, put it in /usr/share/xsessions/, and select it in sessions.
