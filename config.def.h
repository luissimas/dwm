/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include "/home/padawan/.cache/wal/colors-wal-dwm.h"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 5;        /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

static const char *fonts[]          = { "Iosevka Padawan:size=9", "Iosevka Nerd Font:size=9" };

static const char *colors[][3]      = {
	//               fg         bg         border
	[SchemeNorm] = { norm_fg, norm_bg, norm_border },
	[SchemeSel]  = { sel_fg, sel_bg,  sel_border },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
 	{ "[@]",      dwindle },
	{ "[D]",      deck },
	{ "[M]",      monocle },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[]  = { "kitty", NULL };

/* audio commands */
#define volup "pamixer --allow-boost -i 5 && pkill -RTMIN+10 dwmblocks"
#define voldown "pamixer --allow-boost -d 5 && pkill -RTMIN+10 dwmblocks"
#define volmute "pamixer -t && pkill -RTMIN+10 dwmblocks"

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_minus,  incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_equal,  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
  /* Launch */
	{ MODKEY|ShiftMask,    	        XK_t,      spawn,     	   SHCMD("~/scripts/theme-switcher.sh") },
	{ MODKEY,             	        XK_a,      spawn,     	   SHCMD("maim -su | xclip -selection clipboard -t image/png") },
	{ MODKEY|ShiftMask,             XK_e,      spawn,     	   SHCMD("~/scripts/emacs.sh") },
	{ MODKEY|ShiftMask,             XK_q,      spawn,     	   SHCMD("~/scripts/power-prompt.sh") },
	{ MODKEY|ShiftMask,             XK_w,      spawn,     	   SHCMD("$BROWSER") },
	{ MODKEY|ShiftMask,             XK_z,      spawn,     	   SHCMD("zathura") } ,
	/* Volume */
	{ MODKEY|ShiftMask,             XK_equal,  spawn,     	   SHCMD(volup) },
	{ MODKEY|ShiftMask,             XK_minus,  spawn,          SHCMD(voldown) },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD(volmute) },
	/* Layout */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
 	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY|ShiftMask,		          XK_comma,  cyclelayout,    {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	/* Monitors */
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
  /* Laptop media keys */
  { 0,  XF86XK_MonBrightnessUp,               spawn,		SHCMD("light -A 10") },
  { 0,  XF86XK_MonBrightnessDown,             spawn,	  SHCMD("light -U 10") },
  { 0,  XF86XK_AudioRaiseVolume,              spawn,	  SHCMD(volup) },
  { 0,  XF86XK_AudioRaiseVolume,              spawn,	  SHCMD(volup) },
  { 0,  XF86XK_AudioMute,                     spawn,	  SHCMD(volmute) },
	/* Tags */
	{ MODKEY,                       XK_o,   viewnext,       {0} },
	{ MODKEY,                       XK_i,   viewprev,       {0} },
	{ MODKEY|ShiftMask,             XK_o,   tagtonext,      {0} },
	{ MODKEY|ShiftMask,             XK_i,   tagtoprev,      {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
  /* Restarting */
	{ MODKEY|Mod1Mask,		          XK_q,      quit,           {1} },
	{ MODKEY|ShiftMask|Mod1Mask,   	XK_q,      quit,           {0} },
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
