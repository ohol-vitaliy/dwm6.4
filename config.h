/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means standard bar at bottom */
static const int extrabar           = 1;        /* 0 means no extra bar */
static const char statussep         = ';';      /* separator between statuses */
static const char *fonts[]          = { "monospace:size=12" };
static const char dmenufont[]       = "monospace:size=12";
/* static const char col_gray1[]       = "#222222"; */
/* static const char col_gray2[]       = "#444444"; */
/* static const char col_gray3[]       = "#bbbbbb"; */
/* static const char col_gray4[]       = "#eeeeee"; */
/* static const char col_cyan[]        = "#005577"; */
/* static const char *colors[][3]      = { */
/* 	/1*               fg         bg         border   *1/ */
/* 	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 }, */
/* 	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  }, */
/* }; */
static const char normbgcolor[]           = "#fcfdfe";
static const char normbordercolor[]       = "#fcfdfe";
static const char normfgcolor[]           = "#363237";
static const char selfgcolor[]            = "#fcfdfe";
static const char selbordercolor[]        = "#363237";
static const char selbgcolor[]            = "#363237";
static const char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};


/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "st"          , NULL , NULL , 1      , 0 , -1 } ,
	{ "Firefox-esr" , NULL , NULL , 1 << 1 , 0 , -1 } ,
	{ "easyeda"     , NULL , NULL , 1 << 2 , 0 , -1 } ,
	{ "thunar"      , NULL , NULL , 1 << 4 , 0 , -1 } ,
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[t]",      tile }, /* first entry is default */
	{ "[M]",      monocle }, 
	{ "[f]",      NULL },    /* no layout function means floating behavior */
	{ "H[]",      deck },
	/* { "[@]",      spiral }, */
	/* { "[\\]",     dwindle }, */
	/* { "TTT",      bstack }, */
	/* { "===",      bstackhoriz }, */
	/* { "HHH",      grid }, */
	/* { "###",      nrowgrid }, */
	/* { "---",      horizgrid }, */
	/* { ":::",      gaplessgrid }, */
	/* { "|M|",      centeredmaster }, */
	/* { ">M>",      centeredfloatingmaster }, */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char term_cmd[]  = "st";
static const char blueman_cmd[]  = "blueman-manager";
static const char browser_cmd[] = "firefox";
static const char lang_us_cmd[] = "setxkbmap us && echo 'lang: us' | dzen2 -p 1";
static const char lang_ua_cmd[] = "setxkbmap ua && echo 'lang: ua' | dzen2 -p 1";
static const char lang_ru_cmd[] = "setxkbmap ru && echo 'lang: ru' | dzen2 -p 1";
static const char bright_up_cmd[] = "brightnessctl set 5%+ && bash /home/zeroring/.config/scripts/bright | dzen2 -p 1";
static const char bright_down_cmd[] = "brightnessctl set 5%- && bash /home/zeroring/.config/scripts/bright | dzen2 -p 1";
static const char vol_mute_cmd[] = "pulsemixer --toggle-mute && bash /home/zeroring/.config/scripts/volume | dzen2 -p 1";
static const char vol_up_cmd[] = "pulsemixer --change-volume +2 && bash /home/zeroring/.config/scripts/volume | dzen2 -p 1";
static const char vol_down_cmd[] = "pulsemixer --change-volume -2 && bash /home/zeroring/.config/scripts/volume | dzen2 -p 1";


static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ Mod4Mask,                     XK_b,      spawn,          SHCMD(blueman_cmd) },
	{ Mod4Mask,                     XK_f,      spawn,          SHCMD(browser_cmd) },
	{ Mod4Mask,                     XK_Return, spawn,          SHCMD(term_cmd) },
	{ MODKEY,                       XK_w,      spawn,          { .v = dmenucmd } },
	{ MODKEY,                       XK_e,      spawn,          { .v = dmenucmd } },
	{ ControlMask,                  XK_1,      spawn,          SHCMD(lang_us_cmd) },
	{ ControlMask,                  XK_2,      spawn,          SHCMD(lang_ua_cmd) },
	{ ControlMask,                  XK_3,      spawn,          SHCMD(lang_ru_cmd) },
	{ 0,             XF86XK_MonBrightnessUp,   spawn,          SHCMD(bright_up_cmd) },
	{ 0,             XF86XK_MonBrightnessDown, spawn,          SHCMD(bright_down_cmd) },
	{ 0,             XF86XK_AudioMute,         spawn,          SHCMD(vol_mute_cmd) },
	{ 0,             XF86XK_AudioRaiseVolume,  spawn,          SHCMD(vol_up_cmd) },
	{ 0,             XF86XK_AudioLowerVolume,  spawn,          SHCMD(vol_down_cmd) },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY|ShiftMask,             XK_b,      toggleextrabar, {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_g,      togglegaps,     {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          SHCMD(term_cmd) },
	{ ClkExBarLeftStatus,   0,              Button2,        spawn,          SHCMD(term_cmd) },
	{ ClkExBarMiddle,       0,              Button2,        spawn,          SHCMD(term_cmd) },
	{ ClkExBarRightStatus,  0,              Button2,        spawn,          SHCMD(term_cmd) },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

