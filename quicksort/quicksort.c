#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef struct __node {
    struct list_head list;
    long value;
} node_t;

// from q_new
struct list_head *list_new()
{
    struct list_head *new = malloc(sizeof(struct list_head));
    if (!new)
        return NULL;
    INIT_LIST_HEAD(new);
    return new;
}

/* Return number of elements in queue */

int list_size(struct list_head *head)
{
    if (!head)
        return 0;

    int size = 0;
    struct list_head *cur;
    list_for_each (cur, head)
        size++;
    return size;
}

/* Free all storage used by queue */
void list_free(struct list_head *head)
{
    if (!head)
        return;
    node_t *node, *temp;
    list_for_each_entry_safe (node, temp, head, list) {
        list_del(&node->list);
        free(node);
    }
    free(head);
}

bool list_insert_head(struct list_head *head, long n)
{
    if (!head)
        return false;
    node_t *node = malloc(sizeof(node_t));
    if (!node)
        return false;
    node->value = n;
    if (!node->value) {
        return false;
    }
    list_add(&node->list, head);
    return true;
}

/* Verify if list is order */
static bool list_is_ordered(struct list_head *head)
{
    if (!head || list_empty(head))
        return true;
    node_t *entry, *safe;

    list_for_each_entry_safe (entry, safe, head, list) {
        if (entry->value > safe->value)
            return false;
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

void show(struct list_head *head)
{
    struct list_head *curr = head->next;
    while (curr != head) {
        printf("%ld ", list_entry(curr, node_t, list)->value);
        curr = curr->next;
    }
    fprintf(stdout, "\n");
}

void quick_sort(struct list_head **head)
{
    if (!*head || list_empty(*head))
        return;
    int n = list_size(*head);
    int i = 0;
    int max_level = 2 * n;
    struct list_head *begin[max_level];

    for (int i = 1; i < max_level; i++)
        begin[i] = list_new();
    struct list_head *result = list_new(), *left = list_new(),
                     *right = list_new();
    begin[0] = *head;

    int stage = 0;
    // printf("%ld\n", list_entry(begin[0]->prev, node_t, list)->value);
    while (i >= 0) {
        
        fprintf(stdout, "This is %d stage -> ", stage++);

        if (begin[i]->next != begin[i]->prev) {
            fprintf(stdout, "If\n");
            
            node_t *pivot = list_entry(begin[i]->next, node_t, list);
            node_t *entry, *safe;
            list_for_each_entry_safe (entry, safe, begin[i], list) {
                if (entry->value > pivot->value) {
                    list_del(&entry->list);
                    list_add(&entry->list, right);
                } else if (entry->value < pivot->value) {
                    list_del(&entry->list);
                    list_add(&entry->list, left);
                }
            }
            begin[i] = left;
            list_del(&pivot->list);
            list_add(&pivot->list, begin[i + 1]);
            begin[i + 2] = right;

            INIT_LIST_HEAD(left);
            INIT_LIST_HEAD(right);
            i += 2;
        } else {
            fprintf(stdout, "else \n");
            if(!list_empty(begin[i]))
                list_splice_init(begin[i], result);
            i--;
        }
    }
    
    fprintf(stdout, "Sort Complete !\n");
    for (int j = 1; j < max_level; j++) {
        list_free(begin[j]);
    }
    struct list_head *q = result;
    list_for_each(q, result){
        printf("%ld\n",list_entry(q,node_t,list)->value);
    }
    // list_free(left);
    // show(result);
    // list_free(right);
    // show(result);
    *head = result;
}



int main(int argc, char **argv)
{
    struct list_head *list = list_new();
    size_t count = 5;

    int *test_arr = malloc(sizeof(int) * count);

    for (int i = 0; i < count; ++i)
        test_arr[i] = i;
    shuffle(test_arr, count);

    while (count--)
        list_insert_head(list, test_arr[count]);

    show(list);
    quick_sort(&list);

    assert(list_is_ordered(list));

    list_free(list);

    free(test_arr);

    return 0;
}