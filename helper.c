#include "helper.h"

//custom error function
void error(char *str)
{
    printf("%s", str);
    exit(0);
}

//Comparator function for sorting usign qsort
int cmp_func(const void *first, const void *second)
{
    return ((*(request **)first)->size - (*(request **)second)->size);
}
