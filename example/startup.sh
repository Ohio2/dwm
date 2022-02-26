#!/bin/bash
_here=$HOME/.config/dwm
#xkbmap is used when you want to set the keyboard language
#setxkbmap pl
#xrandr is used when you want multiple monitors in your dwm setup, see: https://dwm.suckless.org/multi-monitor/
#xrandr --output DVI-D-0 --left-of HDMI-0 --auto --rate 75
#xrandr --output HDMI-0 --auto --rate 75
#feh is used to set the wallpaper
feh --bg-scale $_here/wallpaper.*
#dunst is used for notifications
#dunst &
#flameshot is used for screenshots (PrtSc/Scr)
flameshot &
#functions
. $_here/functions
xsetroot_gen
