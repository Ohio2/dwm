#!/bin/bash
_here=$HOME/.config/dwm
XDG_RUNTIME_DIR="/run/user/$(id -u)"
PATH="$PATH:$HOME/.local"
#setxkbmap pl
#xrandr --output DVI-D-0 --left-of HDMI-0 --auto --rate 75
#xrandr --output HDMI-0 --auto --rate 75
feh --bg-scale $_here/wallpaper.*
dunst &
flameshot &
pipewire &
#pamixer --allow-boost &
#picom -b 
. $_here/functions
xsetroot_gen &
