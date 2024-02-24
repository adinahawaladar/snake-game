#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 10

typedef struct {
    int x, y;
} Position;

typedef struct {
    Position head;
    Position body[WIDTH * HEIGHT];
    int length;
    int direction; // 0 - up, 1 - right, 2 - down, 3 - left
} Snake;

typedef struct {
    int x, y;
} Food;

void setup();
void draw();
void input();
void logic();
bool collision();
void game_over();

Snake snake;
Food food;

int main() {
    setup();
    
    while (true) {
        draw();
        input();
        logic();
        if (collision()) {
            game_over();
            break;
        }
        usleep(100000); // Adjust game speed
    }
    
    return 0;
}

void setup() {
    srand(time(NULL)); // Seed the random number generator
    
    // Initialize snake
    snake.length = 1;
    snake.head.x = WIDTH / 2;
    snake.head.y = HEIGHT / 2;
    snake.direction = 1;
    
    // Initialize food
    food.x = rand() % WIDTH;
    food.y = rand() % HEIGHT;
}

void draw() {
    system("clear");
    
    // Draw game board
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                printf("#"); // Draw walls
            } else if (i == snake.head.y && j == snake.head.x) {
                printf("O"); // Draw snake head
            } else if (i == food.y && j == food.x) {
                printf("*"); // Draw food
            } else {
                bool isBodyPart = false;
                for (int k = 0; k < snake.length - 1; k++) {
                    if (i == snake.body[k].y && j == snake.body[k].x) {
                        printf("o"); // Draw snake body
                        isBodyPart = true;
                        break;
                    }
                }
                if (!isBodyPart) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
}

void input() {
    char direction;
    printf("Enter direction (w = up, d = right, s = down, a = left): ");
    scanf(" %c", &direction);
    switch(direction) {
        case 'w':
            if (snake.direction != 2) // Can't go down when moving up
                snake.direction = 0;
            break;
        case 'd':
            if (snake.direction != 3) // Can't go left when moving right
                snake.direction = 1;
            break;
        case 's':
            if (snake.direction != 0) // Can't go up when moving down
                snake.direction = 2;
            break;
        case 'a':
            if (snake.direction != 1) // Can't go right when moving left
                snake.direction = 3;
            break;
        case 'q':
            game_over();
            break;
    }
}

void logic() {
    // Move body segments
    for (int i = snake.length - 1; i > 0; i--) {
        snake.body[i] = snake.body[i - 1];
    }
    snake.body[0] = snake.head;
    
    // Move head
    switch (snake.direction) {
        case 0:
            snake.head.y--;
            break;
        case 1:
            snake.head.x++;
            break;
        case 2:
            snake.head.y++;
            break;
        case 3:
            snake.head.x--;
            break;
    }
    
    // Check for food
    if (snake.head.x == food.x && snake.head.y == food.y) {
        snake.length++;
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;
    }
}

bool collision() {
    // Check if snake hits walls
    if (snake.head.x <= 0 || snake.head.x >= WIDTH - 1 || 
        snake.head.y <= 0 || snake.head.y >= HEIGHT - 1)
        return true;
    
    // Check if snake hits itself
    for (int i = 0; i < snake.length - 1; i++) {
        if (snake.head.x == snake.body[i].x && snake.head.y == snake.body[i].y)
            return true;
    }
    
    return false;
}

void game_over() {
    system("clear");
    printf("Game Over!\n");
    printf("Your score: %d\n", snake.length - 1);
    exit(0);
}
