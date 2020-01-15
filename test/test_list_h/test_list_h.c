#include <list.h>
#include <stdio.h>
#include <malloc.h>

typedef struct
{
    int data;
    list_head_t entry;
} list_t;

int main()
{
    list_t data;
    INIT_LIST_HEAD(&(data.entry));

    printf("create a list [0 1 2 3 4]\n");
    for (int i = 0; i < 5; i++)
    {
        list_t *node = (list_t *)malloc(sizeof(list_t));
        node->data = i;
        list_add_tail(&data.entry, &node->entry);
    }

    list_t *iter;
    list_for_each_entry(iter, &data.entry, entry)
    {
        printf("%d ", iter->data);
        // 0 1 2 3 4
    }
    printf("\n");


    printf("delete element 2 and reverse print\n");
    list_t *tmp;
    list_for_each_entry_safe(iter, tmp, &data.entry, entry)
    {
        if (iter->data == 2)
        {
            list_del(&iter->entry);
            free(iter);
        }
    }

    list_for_each_entry_reverse(iter, &data.entry, entry)
    {
        printf("%d ", iter->data);
        // 4 3 1 0
    }
    printf("\n");

    printf("is empty: %d\n", list_empty(&data.entry));

    printf("delete all\n");
    list_for_each_entry_safe(iter, tmp, &data.entry, entry)
    {
        list_del(&iter->entry);
        free(iter);
    }

    printf("is empty: %d\n", list_empty(&data.entry));
    return 0;
}