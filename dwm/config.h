/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx   = 1;        /* border pixel of windows */
static unsigned int gappx      = 3;        /* gaps between windows */
static int swallowfloating     = 1;        /* 1 means swallow floating windows by default */
static unsigned int snap       = 32;       /* snap pixel */
static int showbar             = 1;        /* 0 means no bar */
static int topbar              = 1;        /* 0 means bottom bar */
static char font[]             = "JetBrainsMono Nerd Font :size=09" ;
static char dmenufont[]        = "JetBrainsMono Nerd Font :size=09" ;
static const char *fonts[]     = { font };
static char normbgcolor[]      = "#222222"; // gray1
static char normbordercolor[]  = "#444444"; // gray2
static char normfgcolor[]      = "#bbbbbb"; // gray3
static char selfgcolor[]       = "#eeeeee"; // gray4
static char selbordercolor[]   = "#005577"; // cyan
static char selbgcolor[]       = "#005577"; // cyan
static const float baralpha_bg = 0.8;
static const float baralpha_fg = 1.0;
static const float borderalpha = 1.0;

static const char *colors[][3] = {
	/*               fg           bg           border          */
    [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
    [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};
static float alphas[][3] = {
    /*               fg           bg           border     */
    [SchemeNorm] = { baralpha_fg, baralpha_bg, borderalpha },
    [SchemeSel]  = { baralpha_fg, baralpha_bg, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class          instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
    //{ "Gimp",         NULL,     NULL,           0,         1,          0,           0,        -1 },
    //{ "Firefox",      NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
    { "St",           NULL,     NULL,           0,         0,          1,           0,        -1 },	
    { NULL,           NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
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

/* Terminal Emulator */
#define TERMINAL "st"
static const char *termcmd[]  = { TERMINAL, NULL };

/* Control Media Players */
static const char *medplaypausecmd[] = { "playerctl", "play-pause", NULL };
static const char *mednextcmd[] = { "playerctl", "next", NULL };
static const char *medprevcmd[] = { "playerctl", "previous", NULL };
static const char *mutecmd[] = { "sh", "-c", "wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -35 $(pidof dwmblocks)", NULL };
static const char *volupcmd[] = { "sh", "-c", "wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%+; kill -35 $(pidof dwmblocks)", NULL };
static const char *voldowncmd[] = { "sh", "-c", "wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%-; kill -35 $(pidof dwmblocks)", NULL };

/* Brightness */
static const char *brightness_up[]   = { "brightnessctl","s","5%+", NULL };
static const char *brightness_down[] = { "brightnessctl","s","5%-", NULL };

/* Picom */
static const char *kill_picom[] = { "pkill", "picom", NULL};
static const char *start_picom[] = { "picom", NULL };

/* Screenshot */
static const char *screenshot[] = { "screenshot", NULL};
static const char *screenshot_select[] = { "screenshot", "select", NULL };

/* Keyboard Layout */   
static const char *keyboard_layout[] = { "change_keyboard_layout", NULL };
static const char *dmenu_keyboard_layout[] = { "dmenu_change_keyboard_layout", NULL };

/* Lock screen */
static const char *locker[] = { "slock", NULL };

/* Wallpaper */
static const char *dmenu_wallpaper[] = { "dmenu_change_wallpaper", NULL };

/* Web Browser */
static const char *browser[] = { "firefox", NULL };

/* Text Editor */
static const char *editor[] = { TERMINAL,"-e", "nvim", NULL };

/* File Manager */
static const char *file_manager[] = { TERMINAL, "-e", "lfub", NULL };

/* Colorscheme */
static const char *dmenu_colorscheme[] = { "dmenu_change_colorscheme", NULL };

/* Dmenu */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    { "font",               STRING,  &font },
    { "dmenufont",          STRING,  &dmenufont },
    { "normbgcolor",        STRING,  &normbgcolor },
    { "normbordercolor",    STRING,  &normbordercolor },
    { "normfgcolor",        STRING,  &normfgcolor },
    { "selbgcolor",         STRING,  &selbgcolor },
    { "selbordercolor",     STRING,  &selbordercolor },
    { "selfgcolor",         STRING,  &selfgcolor },
    { "borderpx",           INTEGER, &borderpx },
    { "snap",               INTEGER, &snap },
    { "showbar",            INTEGER, &showbar },
    { "topbar",             INTEGER, &topbar },
    { "nmaster",            INTEGER, &nmaster },
    { "resizehints",        INTEGER, &resizehints },
    { "mfact",              FLOAT,   &mfact },
    { "gappx",              FLOAT,   &gappx },
    { "baralpha_fg",        FLOAT,   &alphas[SchemeNorm][0] }, 
    { "baralpha_bg",        FLOAT,   &alphas[SchemeNorm][1] }, 
    { "borderalpha",        FLOAT,   &alphas[SchemeNorm][2] }, 
    { "baralpha_fg",        FLOAT,   &alphas[SchemeSel][0] },  
    { "baralpha_bg",        FLOAT,   &alphas[SchemeSel][1] },  
    { "borderalpha",        FLOAT,   &alphas[SchemeSel][2] },
};

static const Key keys[] = {
    /* modifier                     key                       function          argument */
    { MODKEY,                       XK_d,                     spawn,            {.v = dmenucmd } },
    { MODKEY,	                    XK_Return,                spawn,            {.v = termcmd }  },
    { MODKEY, 			            XK_b,                     spawn,            {.v = browser }  },
    { MODKEY, 			            XK_r,                     spawn,            {.v = file_manager }  },
    { MODKEY,                       XK_e,                     spawn,            {.v = editor} },
    { 0,                            XK_Print,                 spawn,            {.v = screenshot } },
    { MODKEY,                       XK_Print,                 spawn,            {.v = screenshot_select } },
    { MODKEY,                       XK_space,                 spawn,            {.v = keyboard_layout } },
    { MODKEY|ControlMask,           XK_space,                 spawn,            {.v = dmenu_keyboard_layout } },
    { MODKEY|ShiftMask,             XK_p,                     spawn,            {.v = kill_picom } },
    { MODKEY|ControlMask,           XK_p,                     spawn,            {.v = start_picom } },
    { MODKEY,                       XK_s,                     spawn,            {.v = locker } },
    { MODKEY,                       XK_w,                     spawn,            {.v = dmenu_wallpaper } },
    { MODKEY,                       XK_c,                     spawn,            {.v = dmenu_colorscheme } },
    { MODKEY|ShiftMask,             XK_b,                     togglebar,        {0} },
    { MODKEY,                       XK_j,                     focusstack,       {.i = +1 } },
    { MODKEY,                       XK_k,                     focusstack,       {.i = -1 } },
    { MODKEY,                       XK_h,                     focusstack,       {.i = +1 } },
    { MODKEY,                       XK_l,                     focusstack,       {.i = -1 } },
    { MODKEY,                       XK_i,                     incnmaster,       {.i = +1 } },
    { MODKEY,                       XK_p,                     incnmaster,       {.i = -1 } },
    { MODKEY|ShiftMask,             XK_h,                     setmfact,         {.f = -0.02 } },
    { MODKEY|ShiftMask,             XK_l,                     setmfact,         {.f = +0.02 } },
    { MODKEY|ControlMask,           XK_j,                     pushdown,         {0} },
    { MODKEY|ControlMask,           XK_k,                     pushup,           {0} },
    { MODKEY|ShiftMask,             XK_Return,                zoom,             {0} },
    { MODKEY,                       XK_Tab,                   view,             {0} },
    { MODKEY,	                    XK_q,                     killclient,       {0} },
    { MODKEY,                       XK_t,                     setlayout,        {.v = &layouts[0]} },
    { MODKEY,                       XK_f,                     setlayout,        {.v = &layouts[1]} },
    { MODKEY,                       XK_m,                     setlayout,        {.v = &layouts[2]} },
    { MODKEY|ShiftMask,             XK_v,                     setlayout,        {0} },
    { MODKEY,                       XK_v,                     togglefloating,   {0} },
    { MODKEY|ShiftMask,             XK_f,                     togglefullscr,    {0} },
    { MODKEY|ControlMask,           XK_f,                     fullscreen,       {0} },
    { MODKEY,                       XK_0,                     view,             {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,                     tag,              {.ui = ~0 } },
    { MODKEY,                       XK_comma,                 focusmon,         {.i = -1 } },
    { MODKEY,                       XK_period,                focusmon,         {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,                 tagmon,           {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period,                tagmon,           {.i = +1 } },
    { MODKEY,                       XK_x,                     movecenter,       {0} },
    { MODKEY,                       XK_minus,                 setgaps,          {.i = -1 } },
    { MODKEY,                       XK_equal,                 setgaps,          {.i = +1 } },
    { MODKEY|ShiftMask,             XK_equal,                 setgaps,          {.i = 0  } },
    { 0,                            XF86XK_AudioMute,         spawn,            {.v = mutecmd } },
    { 0,                            XF86XK_AudioLowerVolume,  spawn,            {.v = voldowncmd } },
    { 0,                            XF86XK_AudioRaiseVolume,  spawn,            {.v = volupcmd } },
    { 0,                            XF86XK_AudioPlay,         spawn,            {.v = medplaypausecmd } },
    { 0,                            XF86XK_AudioNext,         spawn,            {.v = mednextcmd } },
    { 0,                            XF86XK_AudioPrev,         spawn,            {.v = medprevcmd } },
    { 0,                            XF86XK_MonBrightnessUp,   spawn,            {.v = brightness_up } },
    { 0,                            XF86XK_MonBrightnessDown, spawn,            {.v = brightness_down } },
    TAGKEYS(                        XK_1,                                       0)
    TAGKEYS(                        XK_2,                                       1)
    TAGKEYS(                        XK_3,                                       2)
    TAGKEYS(                        XK_4,                                       3)
    TAGKEYS(                        XK_5,                                       4)
    TAGKEYS(                        XK_6,                                       5)
    TAGKEYS(                        XK_7,                                       6)
    TAGKEYS(                        XK_8,                                       7)
    TAGKEYS(                        XK_9,                                       8)
    { MODKEY|ShiftMask,             XK_q,                     quit,             {1} }, // with restore
    { MODKEY|ControlMask,           XK_q,                     quit,             {0} }, // without restore
    { MODKEY,                       XK_apostrophe,            scratchpad_show,  {0} },
    { MODKEY|ShiftMask,             XK_apostrophe,            scratchpad_hide,  {0} },
    { MODKEY,                       XK_backslash,             scratchpad_remove,{0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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

