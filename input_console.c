#include "input_console.h"

void checkInput(Snake *snake) {
    if (_kbhit()) {
        int c = _getch();
        if (c == 0 || c == 0xE0) {
            c = _getch();
            if (c == 72 && snake->nextDirection != DOWN) {
                snake->nextDirection = UP; 
                return;
            }
            if (c == 80 && snake->nextDirection != UP) { 
                snake->nextDirection = DOWN; 
                return;
            }
            if (c == 75 && snake->nextDirection != RIGHT) { 
                snake->nextDirection = LEFT; 
                return;
            }
            if (c == 77 && snake->nextDirection != LEFT) { 
                snake->nextDirection = RIGHT;
                return;
            }
        } else {                            
            if ((c == 'w' || c == 'W') && snake->nextDirection != DOWN) {
                snake->nextDirection = UP;
                return;
            }
            if ((c == 's' || c == 'S') && snake->nextDirection != UP) { 
                snake->nextDirection = DOWN;
                return;
            }
            if ((c == 'a' || c == 'A') && snake->nextDirection != RIGHT) { 
                snake->nextDirection = LEFT;
                return;
            }
            if ((c == 'd' || c == 'D') && snake->nextDirection != LEFT) { 
                snake->nextDirection = RIGHT;
                return;
            }
        }
    }
}
