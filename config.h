/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int highlightline  = 1;    /* Use a line to highlight active monitor */
static const unsigned int highlightlineh = 1;    /* Height of the highlight line */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;
static const unsigned int systrayspacing = 0;
static const int systraypinningfailfirst = 1;
static const int showsystray        = 1;
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#61741B";
static char selbgcolor[]            = "#61741B";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	// { "Gimp",     NULL,       NULL,       0,            1,           -1 },
	// { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
    // { "Tilda",    NULL,       NULL,       0,            1,           -1 },
    { "Thunderbird", NULL,       NULL,       1 << 8,       0,           -1 },
    { "kitty",       "float",    NULL,       0,            1,           -1 },
    { NULL, "FLOATME_DWM",       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#include "deck_layout.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[M]",      monocle },
	{ "[]=",      tile },    /* first entry is default */
    { "[][]",     deck },
	{ "><>",      NULL },    /* no layout function means floating behavior */
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

const char *dmenuargs[] = { "/usr/bin/dmenu", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor};
/* commands */
static const char *samecwdterm[]          = { "/home/may/Utility/spawn_terminal.sh", NULL };
static const char *browsercmd[]           = { "firefox", NULL};
static const char *exitcmd[]              = {"killall", "run_loop.sh", NULL};

/* Screenshot commands */
static const char *screenshot_selcmd[]    = {"/home/may/Utility/screenshot.sh", "selection", NULL};
static const char *screenshot_wincmd[]    = {"/home/may/Utility/screenshot.sh", "window", NULL};
static const char *screenshot_choocmd[]   = {"/home/may/Utility/screenshot.sh", NULL};
static const char *screenshot_selshowcmd[]= {"/home/may/Utility/screenshot.sh", "sel_and_show", NULL};

static const char *exitdialogcmd[]        = {"/home/may/Utility/exit_dialog.sh", NULL};
static const char *windowswitchercmd[]    = {"/home/may/Utility/window_switcher.sh", NULL};
static const char *openfilecmd[]          = {"/home/may/Utility/open_file.sh", NULL};
static const char *backlightcmd[]         = {"/home/may/Utility/backlight_menu.sh", NULL};
static const char *doanythingcmd[]        = {"/home/may/Utility/do_anything.sh", NULL};

static const char *toggleRedshiftcmd[]    = {"pkill", "-x", "-USR1", "redshift", NULL};

static void dmenuDesktop(const Arg *arg) {
    char j4dmenuarg[256];
    Arg sarg;

    // Use j4-dmenu-desktop to quickly display a launcher for desktop files
    snprintf(j4dmenuarg, 256, "--dmenu=dmenu -i -m \"%d\" -fn \"%s\" -nb \"%s\" -nf \"%s\" -sb \"%s\" -sf \"%s\"",
		selmon->num, dmenufont, normbgcolor, normfgcolor, selbordercolor, selfgcolor);
    char *j4cmd[] = { "j4-dmenu-desktop", j4dmenuarg, NULL };

    sarg.v = j4cmd;
    spawn(&sarg);
}


static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+5%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-5%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL };

/* Add to keys[] array. With 0 as modifier, you are able to use the keys directly. */
#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      dmenuDesktop,   {0} },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = samecwdterm } },
	{ MODKEY,                       XK_F1,     spawn,          {.v = browsercmd } },

	{ MODKEY,                       XK_b,      togglebar,      {0} },

	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = -1 } },
 	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
 	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
 	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },

	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ControlMask,           XK_space,  setlayout,      {0} },
	{ MODKEY,                       XK_space,  switchcol,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },

	{ MODKEY,                       XK_s,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_s,      tagmon,         {.i = +1 } },

	{ MODKEY,                       XK_c,      centerMouse,    {0} },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	{ MODKEY|ControlMask,           XK_q,      spawn,          {.v = exitcmd} },
	{ MODKEY|ControlMask,           XK_r,      quit,           {0} },
    { MODKEY,                       XK_e,      spawn,          {.v = exitdialogcmd}},

    { MODKEY,                       XK_Print,  spawn,          {.v = screenshot_selcmd} },
    { 0,                            XK_Print,  spawn,          {.v = screenshot_wincmd} },
    { MODKEY|ShiftMask,             XK_Print,  spawn,          {.v = screenshot_choocmd} },
    { MODKEY|ControlMask,           XK_Print,  spawn,          {.v = screenshot_selshowcmd} },

    { MODKEY,                       XK_w,      spawn,          {.v = windowswitchercmd}},
    { MODKEY,                       XK_o,      spawn,          {.v = openfilecmd}},
    { MODKEY|ShiftMask,             XK_b,      spawn,          {.v = backlightcmd}},
    { MODKEY,                       XK_r,      spawn,          {.v = toggleRedshiftcmd}},
    { MODKEY,                       XK_a,      spawn,          {.v = doanythingcmd}},

	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },

	{ 0,                       XF86XK_AudioLowerVolume, spawn, {.v = downvol } },
	{ 0,                       XF86XK_AudioMute, spawn, {.v = mutevol } },
	{ 0,                       XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
    { ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
    { ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

