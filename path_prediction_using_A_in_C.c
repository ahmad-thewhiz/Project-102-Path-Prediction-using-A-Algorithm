#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// Structure representing a node in the grid
typedef struct Node {
    int x, y;               // Coordinates of the node
    int g, h, f;            // Cost and heuristic values for A* algorithm
    struct Node* parent;    // Parent node in the path
} Node;

// Structure representing a node in the priority queue
typedef struct PriorityQueue {
    Node* node;                 // Node in the priority queue
    struct PriorityQueue* next; // Pointer to the next node in the queue
} PriorityQueue;

// Function to create a new node
Node* createNode(int x, int y, Node* parent, int g, int h) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->x = x;
    newNode->y = y;
    newNode->parent = parent;
    newNode->g = g;
    newNode->h = h;
    newNode->f = g + h;
    return newNode;
}

// Function to calculate the Manhattan distance between two nodes
int manhattanDistance(Node* a, Node* b) {
    return abs(a->x - b->x) + abs(a->y - b->y);
}

// Function to insert a node into the priority queue
PriorityQueue* insert(PriorityQueue* head, Node* node) {
    PriorityQueue* newEntry = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    newEntry->node = node;
    newEntry->next = NULL;

    if (!head) {
        return newEntry;
    }

    if (head->node->f >= node->f) {
        newEntry->next = head;
        return newEntry;
    }

    PriorityQueue* current = head;
    while (current->next && current->next->node->f < node->f) {
        current = current->next;
    }
    newEntry->next = current->next;
    current->next = newEntry;

    return head;
}

// Function to pop the node with the highest priority from the queue
PriorityQueue* pop(PriorityQueue* head, Node** node) {
    if (!head) {
        *node = NULL;
        return NULL;
    }
    PriorityQueue* temp = head;
    *node = head->node;
    head = head->next;
    free(temp);
    return head;
}

// Function to check if the priority queue contains a specific node
bool contains(PriorityQueue* head, Node* node) {
    PriorityQueue* current = head;
    while (current) {
        if (current->node->x == node->x && current->node->y == node->y) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Function to check if a coordinate is valid in the grid
bool isValid(int x, int y, int width, int height) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

// Function to print the path found by A* algorithm
void printPath(Node* node, int width, int height, int** grid) {
    if (!node) {
        printf("No path found!\n");
        return;
    }

    // Store path coordinates in a temporary array in reverse order
    int pathLength = 0;
    Node** reversePath = (Node**)malloc(sizeof(Node*) * width * height);
    while (node) {
        reversePath[pathLength++] = node;
        node = node->parent;
    }

    // Create the path grid
    char** pathGrid = (char**)calloc(height, sizeof(char*));
    for (int i = 0; i < height; i++) {
        pathGrid[i] = (char*)calloc(width, sizeof(char));
    }

    node = reversePath[0];
    while (node) {
        pathGrid[node->x][node->y] = '*';
        node = node->parent;
    }

    // Print the path grid
    printf("Path:\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == 1) {
                printf(" 1 ");
            } else if (pathGrid[i][j] == '*') {
                printf(" * ");
            } else {
                printf(" 0 ");
            }
        }
        printf("\n");
    }

    // Print the path coordinates in the correct order
    printf("\nPath: ");
    for (int i = pathLength - 1; i >= 0; i--) {
        printf("(%d, %d)", reversePath[i]->x, reversePath[i]->y);
        if (i > 0) {
            printf(" -> ");
        }
    }
    printf(" -> Goal\n");

    // Free the temporary array and the path grid
    free(reversePath);
    for (int i = 0; i < height; i++) {
        free(pathGrid[i]);
    }
    free(pathGrid);
}

// Function to add obstacles to the grid
void addObstacles(int** grid, int width, int height) {
    int numObstacles;
    printf("Enter the number of obstacles: ");
    scanf("%d", &numObstacles);

    for (int i = 0; i < numObstacles; i++) {
        int x, y;
        printf("Enter the position of obstacle %d (x y): ", i + 1);
        scanf("%d %d", &x, &y);

        if (isValid(x, y, width, height)) {
            grid[y][x] = 1;
        } else {
            printf("Invalid obstacle position. Ignoring obstacle.\n");
            i--;
        }
    }
}

// Function to perform the A* search algorithm
Node* aStar(int startX, int startY, int goalX, int goalY, int width, int height, int** grid) {
    Node* goalNode = createNode(goalX, goalY, NULL, INT_MAX, 0);
    Node* startNode = createNode(startX, startY, NULL, 0, manhattanDistance(startNode, goalNode));

    PriorityQueue* openList = NULL;
    bool** closedList = (bool**)calloc(height, sizeof(bool*));
    for (int i = 0; i < height; i++) {
        closedList[i] = (bool*)calloc(width, sizeof(bool));
    }

    openList = insert(openList, startNode);

    while (openList) {
        Node* currentNode;
        openList = pop(openList, &currentNode);

        if (currentNode->x == goalNode->x && currentNode->y == goalNode->y) {
            return currentNode;
        }

        closedList[currentNode->y][currentNode->x] = true;

        // Explore neighbors of the current node
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;  // Skip the current node

                int newX = currentNode->x + dx;
                int newY = currentNode->y + dy;

                if (!isValid(newX, newY, width, height) || grid[newY][newX] == 1 || closedList[newY][newX]) {
                    continue;  // Skip invalid or already visited nodes
                }

                // Create the neighbor node
                Node* neighbor = createNode(newX, newY, currentNode, currentNode->g + 1, manhattanDistance(currentNode, goalNode));

                // Check if the neighbor is already in the open list or has a better path
                if (!contains(openList, neighbor) || currentNode->g + 1 < neighbor->g) {
                    openList = insert(openList, neighbor);
                }
            }
        }
    }

    return NULL;  // No path found
}

int main() {
    int width, height;

    printf("Enter the width of the grid: ");
    scanf("%d", &width);
    printf("Enter the height of the grid: ");
    scanf("%d", &height);

    // Create the grid
    int** grid = (int**)calloc(height, sizeof(int*));
    for (int i = 0; i < height; i++) {
        grid[i] = (int*)calloc(width, sizeof(int));
    }

    // Add obstacles to the grid
    addObstacles(grid, width, height);

    int startX, startY, goalX, goalY;
    do {
        printf("Enter the start position (x y): ");
        scanf("%d %d", &startX, &startY);
    } while (!isValid(startX, startY, width, height));

    do {
        printf("Enter the goal position (x y): ");
        scanf("%d %d", &goalX, &goalY);
    } while (!isValid(goalX, goalY, width, height));

    // Run A* algorithm
    Node* path = aStar(startX, startY, goalX, goalY, width, height, grid);

    // Print the resulting path
    printPath(path, width, height, grid);

    // Free the allocated memory
    for (int i = 0; i < height; i++) {
        free(grid[i]);
    }
    free(grid);

    return 0;
}
