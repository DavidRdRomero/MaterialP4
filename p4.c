#include "bstree.h"
#include "maze.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define ERROR -6

int int_cmp(const void *int1, const void *int2)
{

    int i1, i2;

    if (!int1 || !int2)
    {
        return ERROR;
    }

    i1 = *((int *)int1);
    i2 = *((int *)int2);

    if (i1 < i2)
        return -1;
    if (i1 > i2)
        return 1;
    else
        return 0;
}

int int_print(FILE *f, const void *i)
{
    return fprintf(f, "%d ", *(int *)i);
}

int string_cmp(const void *s1, const void *s2)
{
    return strcmp((char *)s1, (char *)s2);
}

int string_print(FILE *f, const void *i)
{
    return fprintf(f, "%s ", (char *)i);
}

int point_cmp(const void *p1, const void *p2)
{

    Point *pt1, *pt2;
    if (!p1 || !p2)
    {
        return ERROR;
    }

    pt1 = (Point *)p1;
    pt2 = (Point *)p2;

    double dist1 = 0, dist2 = 0;

    if (!pt1 || !pt2)
    {
        return ERROR;
    }

    dist1 = sqrt(point_getX(pt1) * point_getX(pt1) + point_getY(pt1) * point_getY(pt1));
    dist2 = sqrt(point_getX(pt2) * point_getX(pt2) + point_getY(pt2) * point_getY(pt2));

    if (dist1 < dist2)
        return -1;
    else if (dist1 > dist2)
        return 1;
    else
    {
        if (point_getSymbol(pt1) < point_getSymbol(pt2))
            return -1;
        else if (point_getSymbol(pt1) > point_getSymbol(pt2))
            return 1;
        else
            return 0;
    }
}

BSTree *tree_read_points_from_file(FILE *pf);

void tree_print_rec();

int main(int argc, char *argv[])
{

    FILE *f = NULL;
    Point *p = NULL;
    BSTree *t = NULL;
    if (argc != 2)
    {
        printf("Numero de argumentos invalido\n");
        return 1;
    }

    f = fopen(argv[1], "r");
    if (f == NULL)
    {
        printf("Error al abrir el archivo\n");
        return 1;
    }

    p = point_new(6, 5, '+');
    if (p == NULL)
    {
        printf("Error al crear puntos\n");
        fclose(f);
        return 1;
    }

    if ((t = tree_read_points_from_file(f)) == NULL)
    {
        printf("Error al leer puntos del fichero\n");
        fclose(f);
        return 1;
    }
    fclose(f);

    printf("Imprimiendo el árbol de %s\n", argv[1]);
    tree_print(t);

    printf("elimina 4 3 + \n");
    tree_remove(t, p);

    tree_print(t);

    point_free(p);
    tree_destroy_full(t);
    return 0;
}

BSTree *tree_read_points_from_file(FILE *pf)
{
    BSTree *t;
    int nnodes = 0, i;
    Status st = OK;
    int x, y;
    char symbol;
    Point *p;

    if (!pf)
    {
        return NULL;
    }

    /* Read number of nodes */
    if (fscanf(pf, "%d\n", &nnodes) != 1)
    {
        return NULL;
    }

    /* Create tree */
    t = tree_init(point_print, point_cmp);
    if (!t)
    {
        return NULL;
    }

    /* Read nodes */
    for (i = 0; i < nnodes && st == OK; i++)
    {
        if (fscanf(pf, "%d %d %c", &x, &y, &symbol) != 3)
        {
            return NULL;
        }
        p = point_new(x, y, symbol);
        if (!p)
        {
            tree_destroy(t);
            return NULL;
        }

        st = tree_insert(t, p);
        if (st == ERROR)
        {
            tree_destroy(t);
            point_free(p);
            return NULL;
        }
    }
    return t;
}