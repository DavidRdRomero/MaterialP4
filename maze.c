#include "maze.h"

struct _Point
{
    int x, y;
    char symbol;

    bool visited; // for DFS
};

struct _Maze
{
    int ncols, nrows;
    Point *in, *out;
    Point **map;
};

Point *point_new(int x, int y, char symbol)
{
    Point *p;
    p = malloc(sizeof(Point));
    if (!p)
        return NULL;
    if (symbol != WALL && symbol != IN && symbol != OUT && symbol != SPACE)
    {
        free(p);
        return NULL;
    }
    p->x = x;
    p->y = y;
    p->symbol = symbol;
    return p;
}

void point_free(Point *p)
{
    free(p);
}

int point_getX(const Point *p)
{
    if (!p)
        return INT_MAX;
    return p->x;
}

int point_getY(const Point *p)
{
    if (!p)
        return INT_MAX;
    return p->y;
}
char point_getSymbol(const Point *p)
{
    if (!p)
        return ERRORCHAR;
    return p->symbol;
}

Status point_setX(Point *p, int x)
{
    if (!p)
        return ERROR;
    if (x < 0)
        return ERROR;

    p->x = x;

    return OK;
}

Status point_setY(Point *p, int y)
{
    if (!p)
        return ERROR;
    if (y < 0)
        return ERROR;

    p->y = y;

    return OK;
}

Status point_setSymbol(Point *p, char c)
{
    if (!p)
        return ERROR;
    if (c != WALL && c != IN && c != OUT && c != SPACE)
        p->symbol = ERRORCHAR;
    else
        p->symbol = c;

    return OK;
}

bool point_getVisited(const Point *p)
{
    if (!p)
        return ERROR;
    return p->visited;
}

Status point_setVisited(Point *p, bool bol)
{
    if (!p || (bol != true && bol != false))
        return ERROR;

    p->visited = bol;
    return OK;
}

Point *point_hardcpy(const Point *src)
{

    if (!src)
        return NULL;

    return point_new(src->x, src->y, src->symbol);
}

bool point_equal(const void *p1, const void *p2)
{
    if (!p1 || !p2)
        return false;

    Point *q1, *q2;

    q1 = (Point *)p1;
    q2 = (Point *)p2;

    if ((q1->x == q2->x) && (q1->y == q2->y) && (q1->symbol == q2->symbol))
        return true;
    else
        return false;
}

int point_print(FILE *pf, const void *p)
{ // Print Stack

    if (!p || !pf)
        return -1;

    Point *q1;

    q1 = (Point *)p;

    if (q1)
        return fprintf(pf, "[(%d, %d): %c]", q1->x, q1->y, q1->symbol);
    else
        return -1;
}

Maze *maze_new(int nrows, int ncols)
{

    Maze *newMaze = NULL;
    int i, j;

    if (!nrows || !ncols)
        return NULL;

    newMaze = (Maze *)malloc(sizeof(Maze));
    if (newMaze == NULL)
        return NULL;
    /*Inicialization of the Maze*/

    newMaze->nrows = nrows;
    newMaze->ncols = ncols;
    newMaze->map = (Point **)malloc(nrows * sizeof(Point *));
    if (newMaze->map == NULL)
    {
        free(newMaze);
        return NULL;
    }
    for (i = 0; i < nrows; i++)
    {
        newMaze->map[i] = (Point *)malloc(ncols * sizeof(Point));
        if (newMaze->map[i] == NULL)
        {
            for (j = 0; j < i; j++)
                free(newMaze->map[j]);
            free(newMaze->map);
            free(newMaze);
            return NULL;
        }
    }
    /*Inicialization of each Point*/
    for (i = 0; i < nrows; i++)
    {
        for (j = 0; j < ncols; j++)
        {
            point_setX(&newMaze->map[i][j], i);
            point_setY(&newMaze->map[i][j], j);
            point_setSymbol(&newMaze->map[i][j], SPACE);
            point_setVisited(&newMaze->map[i][j], false);
        }
    }
    return newMaze;
}
void maze_free(Maze *maze)
{
    int i;
    if (maze)
    {
        for (i = 0; i < maze->nrows; i++)
        {
            if (maze->map[i])
                free(maze->map[i]);
        }
        if (maze->map)
            free(maze->map);
        free(maze);
    }
}

Status maze_setIn(Maze *maze, int x, int y)
{
    if (!maze || maze_checkCoordinates(maze, x, y) == ERROR)
        return ERROR;

    maze->in = &maze->map[x][y];
    if (point_setSymbol(&maze->map[x][y], IN) == ERROR)
        return ERROR;
    return OK;
}

Status maze_setOut(Maze *maze, int x, int y)
{
    if (!maze || maze_checkCoordinates(maze, x, y) == ERROR)
        return ERROR;

    maze->out = &maze->map[x][y];
    if (point_setSymbol(&maze->map[x][y], OUT) == ERROR)
        return ERROR;
    return OK;
}

Point *maze_getIn(const Maze *maze)
{
    if (!maze)
        return NULL;

    return maze->in;

    return NULL; /*No point with symbol IN was found*/
}

Point *maze_getOut(const Maze *maze)
{
    if (!maze)
        return NULL;

    return maze->out;

    return NULL; /*No point with symbol OUT was found*/
}
int maze_getNrows(const Maze *maze)
{
    if (!maze)
        return -1;

    return maze->nrows;
}

int maze_getNcols(const Maze *maze)
{
    if (!maze)
        return -1;

    return maze->ncols;
}

Point *maze_getNeighbor(const Maze *maze, const Point *p, direction dir)
{
    if (!maze || !p)
        return NULL;
    if (maze_checkCoordinates(maze, p->x, p->y) == ERROR)
        return NULL;
    switch (dir)
    {
    case 0:
        if (maze_checkCoordinates(maze, p->x, p->y + 1) == ERROR)
            return NULL;
        return &maze->map[p->x][p->y + 1];
    case 1:
        if (maze_checkCoordinates(maze, p->x - 1, p->y) == ERROR)
            return NULL;
        return &maze->map[p->x - 1][p->y];
    case 2:
        if (maze_checkCoordinates(maze, p->x, p->y - 1) == ERROR)
            return NULL;
        return &maze->map[p->x][p->y - 1];
    case 3:
        if (maze_checkCoordinates(maze, p->x + 1, p->y) == ERROR)
            return NULL;
        return &maze->map[p->x + 1][p->y];
    case 4:
        return &maze->map[p->x][p->y];
    default:
        return NULL;
    }
}

Status maze_checkCoordinates(const Maze *maze, int x, int y)
{
    if (!maze)
        return ERROR;

    if ((x >= 0) && (x <= maze->nrows) && (y >= 0) && (y <= maze->ncols))
        return OK;
    return ERROR;
}
Status maze_setSymbol(const Maze *maze, int x, int y, char sym)
{
    if (!maze || !sym)
        return ERROR;
    if (maze_checkCoordinates(maze, x, y) == ERROR)
        return ERROR;

    if (point_setSymbol(&maze->map[x][y], sym) == ERROR)
        return OK;
    return ERROR;
}

Status maze_setVisited(const Maze *maze, int x, int y, bool visited)
{
    if (!maze || !x || !y || (visited != true && visited != false))
        return ERROR;
    if (maze_checkCoordinates(maze, x, y) == ERROR)
        return ERROR;

    if (point_setVisited(&maze->map[x][y], visited) == ERROR)
        return OK;
    return ERROR;
}

/**/
Point *maze_getPoint(const Maze *maze, int x, int y)
{
    if (!maze)
    {
        return NULL;
    }
    return &maze->map[x][y];
}
char maze_getSymbol(const Maze *maze, int x, int y)
{
    if (!maze)
    {
        return ERRORCHAR;
    }
    char c = point_getSymbol(&maze->map[x][y]);
    if (c == ERRORCHAR)
    {
        return ERRORCHAR;
    }
    return c;
}

bool maze_isVisited(const Maze *maze, int x, int y)
{
    if (!maze || !x || !y)
    {
        return false;
    }
    return maze->map[x][y].visited;
}

int maze_printPoints(FILE *fp, const Maze *maze)
{
    int i, j, nrows, ncols, nprinted = 0, auxprinted = 0;
    if (!fp || !maze)
    {
        return -1;
    }

    nrows = maze_getNrows(maze);
    ncols = maze_getNcols(maze);

    if (nrows == -1 || ncols == -1)
    {
        return -1;
    }

    for (i = 0; i < nrows; i++)
    {
        for (j = 0; j < ncols; j++)
        {
            auxprinted = point_print(fp, &maze->map[i][j]);

            if (auxprinted == -1)
            {
                return -1;
            }
            nprinted += auxprinted;
        }
    }

    return nprinted;
}

int maze_print(FILE *fp, const Maze *maze)
{
    if (!fp || !maze)
    {
        return -1;
    }
    int nrows, ncols, i, j, auxprinted = 0, nprinted = 0;

    nrows = maze_getNrows(maze);
    ncols = maze_getNcols(maze);

    if (nrows == -1 || ncols == -1)
    {
        return -1;
    }

    fprintf(fp, "\nMaze: %i rows %i cols\n", nrows, ncols);

    point_print(fp, maze->in);
    point_print(fp, maze->out);
    fprintf(fp, "\n");

    for (i = 0; i < nrows; i++)
    {
        for (j = 0; j < ncols; j++)
        {
            auxprinted = fprintf(fp, "%c", maze_getSymbol(maze, i, j));

            if (auxprinted == -1)
            {
                return -1;
            }
            nprinted += auxprinted;
        }
        fprintf(fp, "\n");
    }

    return nprinted;
}
Status maze_isvalidChar(char c)
{
    if (c == ERRORCHAR || c == IN || c == OUT || c == WALL || c == SPACE)
        return OK;
    else
        return ERROR;
}
Maze *maze_readFromFile(const char *filename)
{
    FILE *fp = NULL;
    Maze *maze = NULL;
    int nrows = 0, ncols = 0, i, j;
    int hayError = 0;
    char cAux;
    if (!filename)
        return NULL;

    fp = fopen(filename, "r");
    if (fp == NULL)
        return NULL;

    if (fscanf(fp, "%d %d", &nrows, &ncols) != 2)
    {
        fclose(fp);
        return NULL;
    }

    maze = maze_new(nrows, ncols);
    if (maze == NULL)
        hayError = 1;

    for (i = 0; i < nrows && !hayError; i++)
    {
        fscanf(fp, "\n");
        for (j = 0; j < ncols && !hayError; j++)
        {
            if (fscanf(fp, "%c", &cAux) == 1)
            {
                if (maze_isvalidChar(cAux) == OK)
                {
                    if (cAux == IN)
                    {
                        if (maze_setIn(maze, i, j) == ERROR)
                        {
                            hayError = 1;
                        }
                    }
                    else if (cAux == OUT)
                    {
                        if (maze_setOut(maze, i, j) == ERROR)
                        {
                            hayError = 1;
                        }
                    }
                    else if (cAux == ERRORCHAR)
                    {
                        hayError = 1;
                    }
                    else
                        point_setSymbol(&maze->map[i][j], cAux);
                }
                else
                {
                    hayError = 1;
                }
            }
            else
            {
                hayError = 1;
            }
        }
    }

    if (hayError)
    {
        maze_free(maze);
        fclose(fp);
        return NULL;
    }
    else
    {
        fclose(fp);
        return maze;
    }
}
