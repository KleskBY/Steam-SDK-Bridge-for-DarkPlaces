// mazegen.c.h


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 60 // Size of the maze

// Function to initialize the maze grid
void initialize_maze(char maze[][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      maze[i][j] = '#'; // Represent walls with '#'
    }
  }
}

void blot (char maze[][SIZE], int centerx, int centery, int diameterx, int diametery)
{
	int x0 = centerx - diameterx / 2;
	int y0 = centery - diametery / 2;
	int x1 = centery + diameterx / 2;
	int y1 = centery + diametery / 2;
	for (int col = x0; col <= x1; col ++)
		for (int row = y0; row <= y1; row ++) {
			if (!in_range_beyond (1, col, SIZE-2)) continue;
			if (!in_range_beyond (1, row, SIZE-2)) continue;
			maze[row][col] = '.';
		}

}

// Function to randomly choose a cell
int random_cell() {
  return rand() % (SIZE / 2); // Choose a cell within the maze's boundaries
}

// Function to generate the maze using recursive backtracking
void generate_maze(char maze[][SIZE], int row, int col) {
  // Mark the current cell as a path
  maze[row][col] = '.';

  // Randomly choose an order of directions to explore
  int directions[4] = {0, 1, 2, 3}; // 0: up, 1: right, 2: down, 3: left
  for (int i = 0; i < 4; i++) {
    int j = rand() % (4 - i);
    int temp = directions[i];
    directions[i] = directions[j];
    directions[j] = temp;
  }

  // Try each direction
  for (int i = 0; i < 4; i++) {
    int new_row, new_col;
    // Determine the new cell based on the direction
    if (directions[i] == 0) { // Up
      new_row = row - 2;
      new_col = col;
    } else if (directions[i] == 1) { // Right
      new_row = row;
      new_col = col + 2;
    } else if (directions[i] == 2) { // Down
      new_row = row + 2;
      new_col = col;
    } else { // Left
      new_row = row;
      new_col = col - 2;
    }

    // If the new cell is within bounds and is a wall, carve a path
    //if (new_row >= 0 && new_row < SIZE && new_col >= 0 && new_col < SIZE &&
	if (new_row >= 1 && new_row < (SIZE - 1) && new_col >= 1 && new_col < (SIZE - 1) &&
        maze[new_row][new_col] == '#') {
      maze[row - (row - new_row) / 2][col - (col - new_col) / 2] = '.'; // Carve path between cells
      generate_maze(maze, new_row, new_col); // Recursively generate the maze from the new cell
    }
  }
}

// Function to print the maze
void print_maze(char maze[][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      Con_Printf("%c", maze[i][j]);
    }
    Con_Printf(NEWLINE);
  }
}

//// Main function
//int main() {
//  char maze[SIZE][SIZE];
//  // Seed the random number generator
//  srand(time(NULL));
//
//  // Initialize the maze
//  initialize_maze(maze);
//
//  // Generate the maze
//  generate_maze(maze, random_cell(), random_cell());
//
//  // Print the maze
//  print_maze(maze);
//
//  return 0;
//}

void SCR_mazegen_f (cmd_state_t *cmd)
{
	char maze[SIZE][SIZE];
	// Seed the random number generator
	srand(time(NULL));

	// Initialize the maze
	initialize_maze(maze); // SOLID WORLD

	for (int j = 0; j < 3; j ++) {
		int diameter_x = (int)(lhrandom(0, 1) * 20 + 5) ;
		int diameter_y = (int)(lhrandom(0, 1) * 10 + 5) ;
		int xmin = diameter_x + 1; // 60 - 20 + 1 = 
		int xmax = (SIZE - 2) - diameter_x/2 ;
		int ymin = diameter_y + 1; // 60 - 20 + 1 = 
		int ymax = (SIZE - 2) - diameter_y/2;
		int xlen = range_length (xmin, xmax);
		int ylen = range_length (xmin, xmax);

		while (1) {
			int centerx = xmin + (lhrandom(0, 1) * xlen); // center?
			int centery = ymin + (lhrandom(0, 1) * ylen);

			// Does intersect other blot?  Then try again ...

			blot (maze, centerx, centery, diameter_x, diameter_y);
			break;
		}
		//int px = rand() % (SIZE / 2); // Choose a cell within the maze's boundaries
	}

	// Generate the maze
	generate_maze(maze, random_cell(), random_cell()); // CARVE PATHS

	// Print the maze
	print_maze(maze);

	//  return 0;

}
