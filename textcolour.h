#ifndef TEXTCOLOUR_H
#define TEXTCOLOUR_H


#define RESET		0
#define BRIGHT 		1
#define DIM		    2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BG_BLACK    40
#define BLACK 		30
#define RED		    31
#define GREEN		32
#define YELLOW		33
#define BLUE		34
#define MAGENTA		35
#define CYAN		36
#define	WHITE		37

void textcolour(int attr, int fg, int bg);

#endif
