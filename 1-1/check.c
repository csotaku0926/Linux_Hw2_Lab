#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "qsort.h"
#include "cpucycles.h"

/* Verify if list is order */
static bool list_is_ordered(node_t *list)
{       
    bool first = true;
    int value;
    while (list) {
        if (first) {
            value = list->value;
            first = false;
        } else {
            if (list->value < value)
                return false;
            value = list->value;
        }
        list = list->next;
    }
    return true;
}

/* shuffle array, only work if n < RAND_MAX */
void shuffle(int *array, size_t n)
{
    if (n <= 0)
        return;

    for (size_t i = 0; i < n - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

void print_list(node_t *list)
{
    while (list) {
        printf("%ld -> ", list->value);
        list = list->next;
    }
    printf("\n");
}

int cmpfunc (const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int cmpfunc_64t (const void *a, const void *b) {
    return (int)(*(int64_t *)a - *(int64_t *)b);
}

int main(int argc, char **argv)
{
    node_t *list = NULL;

    int test_time = 10;

    for (int i = 10; i <= 100000; i *= 10) {

        size_t count = i;
        printf("Size: %ld\n", count);

        int max_cycles[10];
        int64_t elapsed_time[10];

        for (int j = 0; j < test_time; j++) {

            count = i;

            int *test_arr = malloc(sizeof(int) * count);

            for (int i = 0; i < count; ++i)
                test_arr[i] = i;
            shuffle(test_arr, count);

            while (count--)
                list = list_construct(list, test_arr[count]);

            int64_t before = cpucycles();
            quick_sort(&list, max_cycles+j);
            int64_t after = cpucycles();

            elapsed_time[j] = (after - before);

            assert(list_is_ordered(list));

            list_free(&list);

            free(test_arr);

        }

        qsort(max_cycles, test_time, sizeof(int), cmpfunc);
        qsort(elapsed_time, test_time, sizeof(int64_t), cmpfunc_64t);

        // median
        printf("Max length: %d\n", max_cycles[test_time/2]);
        printf("Elapsed Cycles: %ld\n\n", elapsed_time[test_time/2]);
    }
    

    return 0;
}