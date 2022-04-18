#include "movestack.c"
//#include "fibonacci.c"
/* See LICENSE file for copyright and license details. */

/* appearance */
static const int user_bh            = 24;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int startwithgaps	     = 1;	 /* 1 means gaps are used by default */
static const unsigned int gappx     = 10;       /* default gap between windows in pixels */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=14" };
static const char dmenufont[]       = "monospace:size=14";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_gray5[]       = "#303030";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_gray5, col_gray5  },
};
/* autostart */
static const char *const autostart[] = {
	"sh", "-c", "$HOME/.config/dwm/startup.sh", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
/* rules */
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	//{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	//{ "Firefox",  NULL,       NULL,       ,       0,           -1 },
	{ NULL, NULL, NULL, NULL, NULL}
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	//{ "[@]",      spiral },
	//{ "[\\]",      dwindle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALT Mod1Mask
#define PRTSCR	    0x0000ff61
/* Tag Keys */
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ ALT|ControlMask,              KEY,      focusnthmon,    {.i  = TAG } }, \
        { ALT|ShiftMask,                KEY,      tagnthmon,      {.i  = TAG } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_gray5, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *browser[] = { "firefox", NULL };
//static const char *flameshot[] = { "flameshot", "gui", NULL };
static const char *upvol[]   = { "/usr/bin/pamixer", "--sink", "alsa_output.pci-0000_01_00.1.hdmi-stereo-extra1", "--allow-boost","-i", "5",  NULL };
static const char *downvol[] = { "/usr/bin/pamixer", "--sink", "alsa_output.pci-0000_01_00.1.hdmi-stereo-extra1", "--allow-boost", "-d", "5",  NULL };
static const char *mutevol[] = { "/usr/bin/pamixer", "--sink", "alsa_output.pci-0000_01_00.1.hdmi-stereo-extra1", "-t",  NULL };
static const char *resetvol[] = { "/usr/bin/pamixer", "--sink", " alsa_output.pci-0000_01_00.1.hdmi-stereo-extra1", "--set-volume", "100", NULL };
static const char *xkbmapukr[] = { "/usr/bin/setxkbmap", "ru" };
static const char *xkbmappol[] = { "/usr/bin/setxkbmap", "pl" };
static const char *poweroff[] = { "/usr/bin/sudo", "poweroff" };
static const char *screenshotsel[] = {"sh", "-c", "$HOME/.config/dwm/prtscr.sh", "sel", NULL};
static const char *screenshotfull[] = {"sh", "-c", "$HOME/.config/dwm/prtscr.sh", "full", NULL};

/* keys */
/* might be disabled soon since dwmc is implemented (an alternative to dwmfifo)! */
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ ControlMask,                  XK_F1,     spawn,          {.v = upvol}},
	{ ControlMask,                  XK_F2,     spawn,          {.v = downvol}},
	{ ControlMask,                  XK_F3,     spawn,          {.v = mutevol}},
	{ ControlMask,                  XK_F4,     spawn,          {.v = resetvol}},
        { ControlMask,                  XK_F5,     spawn,          {.v = xkbmapukr}},
	{ ControlMask,                  XK_F6,     spawn,          {.v = xkbmappol}},
	{ 0,     			PRTSCR,    spawn,          SHCMD("$HOME/.config/dwm/prtscr.sh sel")},
	{ ShiftMask,			PRTSCR,    spawn,          SHCMD("$HOME/.config/dwm/prtscr.sh full")},
	{ ALT,                          XK_b,      spawn,          {.v = browser } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },	
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
        { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_p, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_y,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_minus,  setgaps,        {.i = GAP_RESET } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = GAP_TOGGLE} },
	{ ControlMask|ALT,              XK_Escape, spawn,          {.v = poweroff}},
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },	
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

void
setlayoutex(const Arg *arg)
{
	setlayout(&((Arg) { .v = &layouts[arg->i] }));
}

void
viewex(const Arg *arg)
{
	view(&((Arg) { .ui = 1 << arg->ui }));
}

void
viewall(const Arg *arg)
{
	view(&((Arg){.ui = ~0}));
}

void
toggleviewex(const Arg *arg)
{
	toggleview(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagex(const Arg *arg)
{
	tag(&((Arg) { .ui = 1 << arg->ui }));
}

void
toggletagex(const Arg *arg)
{
	toggletag(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagall(const Arg *arg)
{
	tag(&((Arg){.ui = ~0}));
}
/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signame> [<type> <value>]"` */
static Signal signals[] = {
        /* signum           function */
        { "focusstack",     focusstack },
        { "setmfact",       setmfact },
        { "togglebar",      togglebar },
        { "incnmaster",     incnmaster },
        { "togglefloating", togglefloating },
        { "focusmon",       focusmon },
        { "tagmon",         tagmon },
        { "zoom",           zoom },
        { "view",           view },
        { "viewall",        viewall },
        { "viewex",         viewex },
        { "toggleview",     view },
        { "toggleviewex",   toggleviewex },
        { "tag",            tag },
        { "tagall",         tagall },
        { "tagex",          tagex },
        { "toggletag",      tag },
        { "toggletagex",    toggletagex },
        { "killclient",     killclient },
        { "quit",           quit },
        { "setlayout",      setlayout },
        { "setlayoutex",    setlayoutex },
	{ "tagnthmon",         tagnthmon },
	{ "focusnthmon",       focusnthmon },
	{ "setgaps",        setgaps },	

};
