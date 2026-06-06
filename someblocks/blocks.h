//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
  /*Icon*/ /*Command*/		        /*Update Interval*/	/*Update Signal*/
  { "",    "sb-pacupdate",        3600,               3},
  { "󰌌 ",  "sb-keyboard-layout",  0,                  2},
  { "  ", "sb-memory",           5,                  0},
  { "",    "sb-volume",           0,                  1},
  { " ",  "sb-clock-date",       360,                0},
  { " ",  "sb-clock-time",       1,                  0}

	
  /* Updates whenever "pkill -SIGRTMIN+10 someblocks" is ran */
  /* {"", "date '+%b %d (%a) %I:%M%p'",					0,		10}, */
};



//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim[] = "   ";
static unsigned int delimLen = 3;
