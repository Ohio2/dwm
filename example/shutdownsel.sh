$_here=$HOME/.config/dwm
 declare -a options=(
     "Lock screen"
     "Logout"
      "Reboot"
      "Shutdown"
      "Suspend"
      "Quit"
  )
  choice=$("dmenu -fn monospace:size=12 -nb #222222 -nf #bbbbbb -sb #303030 -sf #eeeeee -p 'quit menu: ' '${options}' ")
  case $choice in
	  'Lock screen') i3lock -i $_here/wallpaper.png ;;
	  'Logout') killall dwm ;;
	  'Reboot') reboot ;;
	  'Shutdown') poweroff ;;
	  'Suspend') suspend ;;
	  'Quit') exit 1 ;;
  esac
