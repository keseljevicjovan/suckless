#ifndef CONFIG_H
#define CONFIG_H

// String used to delimit block outputs in the status.
#define DELIMITER "   "

// Maximum number of Unicode characters that a block can output.
#define MAX_BLOCK_OUTPUT_LENGTH 45

// Control whether blocks are clickable.
#define CLICKABLE_BLOCKS 0

// Control whether a leading delimiter should be prepended to the status.
#define LEADING_DELIMITER 0

// Control whether a trailing delimiter should be appended to the status.
#define TRAILING_DELIMITER 0

// Define blocks for the status feed as X(icon, cmd, interval, signal).
#define BLOCKS(X)             \
/*X(" ",      "music",            0   ,   4)*/\
/*X(" ",      "cpu-percent",      5   ,   0)*/\
  X("",        "pacupdate",        3600,   3)  \
  X("󰌌 ",      "keyboard-layout",  0,      2)  \
  X("  ",     "memory",        	 5,		   0)  \
/*X("  ",     "memory-percent",	 5,		   0)*/\
  X("",        "volume",           0,      1)  \
  X(" ",      "clock-date",       360,    0)  \
  X(" ",      "clock-time",       1,      0)

#endif  // CONFIG_H
