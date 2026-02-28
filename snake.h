#define STARTING_LENGTH 3

typedef enum {
    RIGHT,
    DOWN,
    LEFT,
    UP
} Direction;

typedef struct {
    int coord_x;
    int coord_y;
} Snake_coord;

typedef struct {
    int length;
    int capacity;
    Direction direction;
    Direction nextDirection;
    bool growPending;
    Snake_coord *body;
} Snake;