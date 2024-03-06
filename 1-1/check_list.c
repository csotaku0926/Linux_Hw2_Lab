#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "qsort_list.h"
#include "cpucycles.h"

/* Verify if list is order */
static bool list_is_ordered(struct list_head *head)
{       
    if (!head)
        return true;

    element_t *node, *safe;
    list_for_each_entry_safe(node, safe, head, list) {
        if (&safe->list == head)
            return true;
        
        if (node->value > safe->value) {
            printf("[Error] : %ld is greater than %ld\n", node->value, safe->value);
            return false;
        }
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


int cmpfunc (const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int cmpfunc_64t (const void *a, const void *b) {
    return (int)(*(int64_t *)a - *(int64_t *)b);
}

int main(int argc, char **argv)
{
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

            struct list_head *list = list_new();
            
            while (count--)
                list = list_construct(list, test_arr[count]);

            int64_t before = cpucycles();
            quick_sort(&list, max_cycles+j);
            int64_t after = cpucycles();

            elapsed_time[j] = (after - before);

            assert(list_is_ordered(list));

            list_free(list);

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