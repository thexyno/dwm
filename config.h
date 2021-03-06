/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrainsMono Nerd Font:size=10" };
static const char dmenufont[]       = "JetBrainsMono Nerd Font:size=10";
static const char col_gray0[]       = "#141414"; // gruvbox bg0
static const char col_gray1[]       = "#282828"; // gruvbox bg0
static const char col_gray2[]       = "#3c3836"; // gruvbox bg1
static const char col_gray3[]       = "#665c54"; // gruvbox bg2
static const char col_gray4[]       = "#928374"; // gruvbox grey
static const char col_cyan[]        = "#19cb00"; // gruvbox purple
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray4, col_gray1, col_gray2 },
	[SchemeSel]  = { col_cyan, col_gray1,  col_gray2  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"/home/philipp/Downloads/Timeular.AppImage", NULL };
const char *spcmd2[] = {"com.bitwarden.desktop", NULL };
// const char *spcmd3[] = {"keepassxc", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"timeular",      spcmd1},
	{"bitwarden",    spcmd2},
// 	{"keepassxc",   spcmd3},
};

static const char *const autostart[] = {
  "setxkbmap", "-option", "caps:swapescape", NULL,                                      // Caps Swap Escape
  "nvidia-settings", "--assign", 
      "CurrentMetaMode=DPY-1: nvidia-auto-select @1050x1680 +0+0 {ForceCompositionPipeline=On, ViewPortIn=1050x1680, ViewPortOut=1680x1050+0+0, Rotation=270}, DPY-2: nvidia-auto-select @2560x1440 +1050+120 {ForceCompositionPipeline=On, ViewPortIn=2560x1440, ViewPortOut=2560x1440+0+0}, DPY-5: nvidia-auto-select @2560x1440 +3610+120 {ForceCompositionPipeline=On, ViewPortIn=2560x1440, ViewPortOut=2560x1440+0+0}",
      NULL,                                                                             // Screen Settings
  "picom", NULL,                                                                        // Compositor
  "sh", "-c", "sleep 1; feh --bg-fill ~/GloryhammerWallpaper/ml4jn9hn7f731.jpg", NULL,  // Wallpaper
  "sh", "-c", "cd /home/philipp/proj/pulse-launch; pipenv run python pulse_launch.py --term_cmd '/home/philipp/scripts/toggleSpeakers turn_off' --other_cmd '/home/philipp/scripts/toggleSpeakers turn_off' 'alsa_output.usb-BEHRINGER_UMC202HD_192k-00.analog-stereo' '/home/philipp/scripts/toggleSpeakers turn_on'", NULL,  // Wallpaper
  "sh", "-c", "/home/philipp/proj/dwmblocks/dwmblocks", NULL,       // Status Bar
  "dunst", NULL,                                                                        // Notification Manager
  "kdeconnectd", NULL,                                                                   // Kdeconnect
  "clipmenud", NULL,                                                                   // Clipmenu
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "floating", NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	{ NULL,       "timeular", NULL,       SPTAG(0),     1,           2 },
	{ NULL,       "bitwarden", NULL,      SPTAG(1),     1,           2 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "Tiling",      tile },    /* first entry is default */
	{ "Floating",      NULL },    /* no layout function means floating behavior */
	{ "Monocle",      monocle },
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
static const char *dmenucmd[] =       { "rofi", "-show", "combi", NULL };
static const char *termcmd[]  =       { "kitty", "-1", NULL };
static const char *termfloatcmd[]  =  { "kitty", "-1", "--class=floating", NULL };
static const char *pulsemixercmd[]  = { "kitty", "-1", "--class=floating", "pulsemixer", NULL };
static const char *nnncmd[]  =        { "kitty", "-1", "--class=floating", "zsh", "-ic", "n;exec zsh", NULL };
static const char *scrotcmd[]  =      { "sh", "-c", "scrot -s  '%Y-%m-%d_\\$wx\\$h_scrot.png' -e 'mv \\$f ~/Screenshots/; xclip -t image/png -selection clipboard ~/Screenshots/\\$f'", NULL };
static const char *volupcmd[]  =      { "bash", "/home/philipp/scripts/changeVolume", "+5", NULL };
static const char *voldowncmd[]  =    { "bash", "/home/philipp/scripts/changeVolume", "-5", NULL };
static const char *volmutecmd[]  =    { "bash", "/home/philipp/scripts/changeVolume", "mute", NULL };
static const char *playpausecmd[]  =  { "playerctl", "play-pause", NULL };
static const char *nextcmd[]  =       { "playerctl", "next", NULL };
static const char *previouscmd[] =    { "playerctl", "previous", NULL };
static const char *clipmenucmd[] =    { "clipmenu", NULL };
static const char *sharenix2cmd[] = { "nextshot", "-a", 0 };
static const char *sharenix3cmd[] = { "nextshot", "-w", 0 };
static const char *sharenix4cmd[] = { "nextshot", "-f", 0 };

static Key keys[] = {
	/* modifier                     key          function        argument */
	{ MODKEY,                       XK_p,        spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,   spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_v,        spawn,          {.v = pulsemixercmd } },
	{ MODKEY|ShiftMask,             XK_f,        spawn,          {.v = nnncmd } },
	{ MODKEY|ControlMask,           XK_Return,   spawn,          {.v = termfloatcmd } },
	{ MODKEY|Mod1Mask,								XK_2,				 spawn,					 {.v = sharenix2cmd } },
  { MODKEY|Mod1Mask,								XK_3,				 spawn,					 {.v = sharenix3cmd } },
  { MODKEY|Mod1Mask, 							XK_4, 			 spawn, 				 {.v = sharenix4cmd } },
	{ MODKEY|ShiftMask,             XK_c,        spawn,          {.v = clipmenucmd } },
	{ 0,                            XK_Print,    spawn,          {.v = scrotcmd } },
  { 0,                            0x1008ff11,  spawn,          {.v = voldowncmd } },
  { 0,                            0x1008ff12,  spawn,          {.v = volmutecmd } },
  { 0,                            0x1008ff13,  spawn,          {.v = volupcmd } },
  { 0,                            0x1008ff14,  spawn,          {.v = playpausecmd } },
  { 0,                            0x1008ff16,  spawn,          {.v = previouscmd } },
  { 0,                            0x1008ff17,  spawn,          {.v = nextcmd } },
	{ MODKEY,											  XK_period,   togglescratch,  {.ui = 0 } },
	{ MODKEY,											  XK_minus,  	 togglescratch,  {.ui = 1 } },
	{ MODKEY,                       XK_s,	       togglesticky,   {0} },
	{ MODKEY,                       XK_b,        togglebar,      {0} },
	{ MODKEY,                       XK_j,        focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,        focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,        incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,        incnmaster,     {.i = -1 } },
//{ MODKEY,                       XK_h,        setmfact,       {.f = -0.05} },
//{ MODKEY,                       XK_l,        setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return,   zoom,           {0} },
	{ MODKEY,                       XK_Tab,      view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,        killclient,     {0} },
	{ MODKEY,                       XK_t,        setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,        setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,        setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,    setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,    togglefloating, {0} },
	{ MODKEY,                       XK_0,        view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,        tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_h,        focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_l,        focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_h,        tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_l,        tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                        0)
	TAGKEYS(                        XK_2,                        1)
	TAGKEYS(                        XK_3,                        2)
	TAGKEYS(                        XK_4,                        3)
	TAGKEYS(                        XK_5,                        4)
	TAGKEYS(                        XK_6,                        5)
	TAGKEYS(                        XK_7,                        6)
	TAGKEYS(                        XK_8,                        7)
	TAGKEYS(                        XK_9,                        8)
	{ MODKEY|ShiftMask,             XK_p,        quit,           {0} },
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

