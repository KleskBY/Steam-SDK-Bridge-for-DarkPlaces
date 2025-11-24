//// mazegen_pathfind_bfs.c.h
//
////#include <stdio.h>
////#include <stdlib.h>
////#include <stdbool.h>
//
//#define MAX_ROWS_256 256
//#define MAX_COLS_256 256
//
//
//typedef struct {
//    point2d_s cells[MAX_ROWS_256 * MAX_COLS_256];
//    int front, rear;
//} Queue;
//
//void initQueue(Queue *q) {
//    q->front = 0;
//    q->rear = -1;
//}
//
//bool isEmpty(Queue *q) {
//    return q->rear < q->front;
//}
//
//void enqueue(Queue *q, point2d_s cell) {
//    q->cells[++q->rear] = cell;
//}
//
//point2d_s dequeue(Queue *q) {
//    return q->cells[q->front++];
//}
//
//qbool isValid(int row, int col, int rows, int cols, char maze[][MAX_COLS_256], qbool visited[][MAX_COLS_256]) {
//	
//    //return (row >= 0 && row < rows && col >= 0 && col < cols && maze[row][col] != '#' && !visited[row][col]);
//	if (!(row >= 0 && row < rows && col >= 0 && col < cols))
//		return false;
//	
//	if (maze[row][col] == '#')
//		return false;
//	
//	if (visited[row][col])
//		return false;
//
//	return true;
//}
//
//
////point2d_s solve_recursive(mazegen_s *m, ccs *s, point2d_s curpos, point2d_s *psolvepos) 
//qbool solveMazeBFS_Not_Recursive(char maze[][MAX_COLS_256], int rows, int cols, point2d_s start, point2d_s end)
//{
//    qbool visited[MAX_ROWS_256][MAX_COLS_256] = {0};
//	//Mem_FreeNull_ (m->visited_za);
//	//m->visited_za = ZAlloc_Char (maze->sizeof_char_ray);
//
//    Queue q;
//    initQueue(&q);
//
//    enqueue(&q, start);
//
//    visited[start.row][start.col] = true;
//
//    int row_moves[] = {-1, 1, 0, 0};
//    int col_moves[] = {0, 0, -1, 1};
//
//    while (!isEmpty(&q)) {
//        point2d_s current = dequeue(&q);
//        if (current.row == end.row && current.col == end.col) {
//            return true; 
//        }
//
//        for (int i = 0; i < 4; i++) {
//            int newRow = current.row + row_moves[i];
//            int newCol = current.col + col_moves[i];
//
//            if (isValid(newRow, newCol, rows, cols, maze, visited)) {
//                point2d_s next = {newRow, newCol};
//                enqueue(&q, next);
//                visited[newRow][newCol] = true;
//            }
//        }
//    }
//    return false;
//}
//
//qbool Maze_SolveBFS(mazegen_s *m, ccs *s, point2d_s startpos, point2d_s endpos) 
//{
//	char mazeo[MAX_ROWS_256][MAX_COLS_256] = {0};
//	for (int row = 0; row < m->rows; row ++) {
//		for (int col = 0; col < m->cols; col ++) {
//			int cell_idx = row * m->rows + col;
//			mazeo[row][col] = s[cell_idx];
//		}
//	}
//
////int main() {
////    char maze[MAX_ROWS][MAX_COLS] = {
////        {'.', '.', '#', '#', '#'},
////        {'#', '.', '.', '#', '#'},
////        {'#', '#', '.', '.', '#'},
////        {'#', '#', '#', '.', '.'}
////    };
////
//    int rows = m->rows;
//    int cols = m->cols;
//point2d_s start = startpos;
//point2d_s end = endpos;
////    point2d_s end = {3, 4};
////
//    if (solveMazeBFS_Not_Recursive(mazeo, rows, cols, start, end)) {
//        Con_PrintLinef ("Maze solved!");
//    } else {
//        Con_PrintLinef ("No path found.");
//    }
//    return 0;
//}

//int idx_to_row (mazegen_s *m, int idx)
//{
//	return idx / m->rows;
//}
//
//int idx_to_col (mazegen_s *m, int idx)
//{
//	return idx % m->rows;
//}
