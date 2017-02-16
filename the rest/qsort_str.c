#include "libft.h"

static void swap(t_list *head, int i, int j)
{
    t_list *tmp = head;
    int tmpicontent_size;
    int tmpjcontent_size;
    int ti = i;
    while(tmp && i) {
        i--;
        tmp = tmp->next;
    }
    tmpicontent_size = tmp->content_size;
    tmp = head;
    while(tmp && j) {
        j--;
        tmp = tmp->next;
    }
    tmpjcontent_size = tmp->content_size;
    tmp->content_size = tmpicontent_size;
    tmp = head;
    i = ti;
    while(tmp && i) {
        i--;
        tmp = tmp->next;
    }
    tmp->content_size = tmpjcontent_size;
}

int get_lcontent_size(t_list *head, int l)
{
    while(head && l) {
        head = head->next;
        l--;
    }
    if (head != NULL)
        return head->content_size;
    else
        return -1;
}

static t_list *Quick_Sort_List(t_list *head, int l, int r)
{
    int i, j;
    int jcontent_size;
    int pivot;
    i = l + 1;
    if (l + 1 < r) {
        pivot = get_lcontent_size(head, l);
        printf("Pivot = %d\n", pivot);
        for (j = l + 1; j <= r; j++) {
            jcontent_size = get_lcontent_size(head, j);
            if (jcontent_size < pivot && jcontent_size != -1) {
                swap(head, i, j);
                i++;
            }
        }
        swap(head, i - 1, l);
        Quick_Sort_List(head, l, i);
        Quick_Sort_List(head, i, r);
    }
    return head;
}

static void print_list(t_list *head)
{
    while(head) {
        printf("%d->", head->content_size);
        head = head->next;
    }
    printf("\n");
}

t_list *Sort_linkedlist(t_list *head)
{
    t_list *tmp = head;

    int n = 0;

    while (tmp) {
        n++;
        tmp = tmp->next;
    }
    printf("n = %d\n", n);
    head = Quick_Sort_List(head, 0, n);
    return head;
}
