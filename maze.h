#ifndef MAZE_H
#define MAZE_H

#include "types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*************************************************************************
 *
 *  Types
 *
 *************************************************************************/

/* START [_Point] */
typedef struct _Point Point;
/* END [_Point] */

/* START [_Maze] */
typedef struct _Maze Maze;
/* END [_Maze] */

/* legal symbols for points */
#define ERRORCHAR 'E'
#define IN 'i'
#define OUT 'o'
#define WALL '+'
#define SPACE '.'

/* enum for neighbors/directions */
typedef enum {
  RIGHT = 0,
  UP = 1,
  LEFT = 2,
  DOWN = 3,
  STAY = 4,
} direction;

/*************************************************************************
 *
 *  Point functions
 *
 *************************************************************************/

/**
 * @brief Constructor. Initialize a point.
 *
 * This function allocates memory for a point and sets its fields acoording
 * its input parameter values.
 *
 * @code
 * // Example of use
 * Point * v;
 * v = point_new (0, 0, WALL);
 * @endcode
 *
 * @param x Point x coordinate
 * @param y Point y coordinate
 * @param symbol Point symbol
 *
 * @return Return the initialized point if it was done correctly,
 * otherwise return NULL. A point is valid if its coordinates are non-negative
 * zero and its symbol is allowed.
 * Allowed symbols are: WALL, IN, OUT or SPACE
 */
Point *point_new(int x, int y, char symbol);

/**
 * @brief Destructor. Free the dynamic memory reserved for a point .
 *
 * @param p Point to free
 */
void point_free(Point *p);

/**
 * @brief Gets the x coordinate of a given point.
 *
 * @param Point pointer
 *
 * @return Returns the x coordinate of a given point, or INT_MAX in
 * case of error.
 */
int point_getX(const Point *p);

/**
 * @brief Gets the y coordinate of a given point.
 *
 * @param Point pointer
 *
 * @return Returns the y coordinate of a given point, or INT_MAX in
 * case of error.
 */
int point_getY(const Point *p);

/**
 * @brief Gets the symbol of a given point.
 *
 * @param Point pointer
 *
 * @return Returns the x coordinate of a given point, or ERROR in
 * case of error.
 */
char point_getSymbol(const Point *p);

/**
 * @brief Modifies the x coordinate  of a given point.
 *
 * @param p Point pointer
 * @param x New x coordinate, must be equal or greater than 0
 *
 * @return Returns OK or ERROR in case of error
 */
Status point_setX(Point *p, int x);

/**
 * @brief Modifies the y coordinate  of a given point.
 *
 * @param p Point pointer
 * @param y New y coordinate, must be equal or greater than 0
 *
 * @return Returns OK or ERROR in case of error
 */
Status point_setY(Point *p, int y);

/**
 * @brief Modifies the symbol of a given point.
 *
 * @param p Point pointer
 * @param c New symbol, must be a valid symbol
 *
 * @return Returns OK or ERRORCHAR in case of error
 */
Status point_setSymbol(Point *p, char c);

bool point_getVisited(const Point *p);       // DFS (P2)
Status point_setVisited(Point *p, bool bol); // DFS (P2)

/**
 * @brief Reserves memory for a point where it copies the data from
 * the point src.
 *
 * @code
 * // Example of use
 * Point *trg, *src;
 * src = point_new();
 * trg = point_hardcpy(src);
 * // .... aditional code ...
 * // free points
 * point_free(src);
 * point_free(trg);
 * @endcode
 *
 * @param src Original Point pointer
 *
 * @return Returns the pointer of the copied Point if everything
 * went well, or NULL otherwise.
 */
Point *point_hardcpy(const Point *src);

/**
 * @brief Compares two points.
 *
 * @param p1,p2 Points to compare.
 *
 * @return Returns True or False.
 * In case of error, returns FALSE.
 */
bool point_equal(const void *p1, const void *p2);

/**
 * @brief Prints in pf the data of a point.
 *
 * The format will be: [(x, y): symbol], without line break at the
 * end.
 *
 * @code
 * Point * v;
 * v = point_new ();
 * point_print (stdout, v);
 * @endcode
 *
 * @param pf File descriptor
 * @param v Point to be printed
 *
 * @return Returns the number of characters that have been written
 * successfully. If there have been errors returns -1.
 */
int point_print(FILE *pf, const void *p); // Print Stack

/*************************************************************************
 *
 *  Maze functions
 *
 *************************************************************************/

/**
 * @brief  Creates a new empty Maze with nrows and ncols.
 *
 * Allocates memory for a new maze and initializes it:
 * - sets ncols and nrows
 * - dynamically allocates a bidimensional array of nrows x ncols for the maze.
 * - initializes all the points in the map with their coordinates, the
 *   SPACE symbol, and visited=false
 *
 * @param nrows, ncols Dimensions of the maze's map
 *
 * @return A pointer to the maze if it was correctly allocated,
 * NULL otherwise.
 **/
Maze *maze_new(int nrows, int ncols);

/**
 * @brief Frees a maze.
 *
 * Frees all the memory allocated for the maze
 *
 * @param g Pointer to maze to be freed.
 **/
void maze_free(Maze *maze);

/**
 * @brief Functions to get and set the entry and exit points of the maze,
 * columns and rows
 */
Status maze_setIn(Maze *maze, int x, int y);
Status maze_setOut(Maze *maze, int x, int y);
Point *maze_getIn(const Maze *maze);
Point *maze_getOut(const Maze *maze);
int maze_getNrows(const Maze *maze);
int maze_getNcols(const Maze *maze);

/**
 * @brief Obtains the neighbor of Point *p in the maze in direction dir
 *
 * @param maze
 * @param p
 * @param dir
 * @return Point*
 */
Point *maze_getNeighbor(const Maze *maze, const Point *p, direction dir);

/* check whether coordinates x and y are valid for maze */
Status maze_checkCoordinates(const Maze *maze, int x, int y);

/**
 * Functions to operate on maze points (self-explanatory)
 **/
Status maze_setSymbol(const Maze *maze, int x, int y, char sym);
Status maze_setVisited(const Maze *maze, int x, int y, bool visited);
Point *maze_getPoint(const Maze *maze, int x, int y);
char maze_getSymbol(const Maze *maze, int x, int y);
bool maze_isVisited(const Maze *maze, int x, int y);

/**
 * @brief prints the points of a maze's map by calling print_point for
 * every point
 *
 * @param fp
 * @param maze
 * @return int, the number of bytes printed
 *
 * For example, the map
 * ++++
 * +io+
 * ++++
 * is printed as
 * [(0, 0): +][(0, 1): +][(0, 2): +][(0, 3): +][(1, 0): +][(1, 1): i]
 * [(1, 2): o][(1, 3): +][(2, 0): +][(2, 1): +][(2, 2): +][(2, 3): +]
 */
int maze_printPoints(FILE *fp, const Maze *maze);

/* START [maze_print] */
/**
 * @brief prints a graphical representation a maze's map, similar
 * to the one processed by maze_ReadFromFile
 *
 * @param fp
 * @param maze
 * @return int the number of bytes printed
 *
 * Sample output:
Reading maze from laberinto_1.txt
Maze: 4 rows 5 cols
[(1, 3): i][(2, 1): o]
+++++
+..i+
+o..+
+++++
*/
int maze_print(FILE *fp, const Maze *maze);

/* START [maze_readFromFile] */
/**
 * @brief Reads a map definition from a text file.
 *
 * Reads a maze description from the text file filename and returns the maze,
 * This function allocates memory for the maze
 *
 * The file contains a header with the map number of rows and cols.
 * The next file lines corresponds with the points of the map.
 *.
 * For example the file:
 *
 * 3 4
 * ++++
 * +io+
 * ++++
 *
 * Represents a map with 3 rows, 4 columns and the points:
 * [(0, 0): +][(0, 1): +][(0, 2): +][(0, 3): +][(1, 0): +][(1, 1): i]
 * [(1, 2): o][(1, 3): +][(2, 0): +][(2, 1): +][(2, 2): +][(2, 3): +]
 *
 * @param filename, name of text file
 *
 * @return the newly allocated maze or NULL if there is any error
 */
Maze *maze_readFromFile(const char *filename);

#endif /* MAZE_H */
