#include "eLists.h"

struct sList* sList_cons()
{
    struct sList* list = calloc(sizeof(struct sList), 1);
    list->len = 0;
    list->first = NULL;

    list->push = &sList_push;
    list->pop = &sList_pop;
    list->get = &sList_get;

    list->begin = &sList_begin;
    list->next = &sList_next;
    list->done = &sList_done;

    return list;
}

void sList_decons(struct sList** list)
{
    while((*list)->len > 0)
        (*list)->pop(*list, 1);
    free(*list);
    *list = NULL;
}

void sList_push(struct sList* list, void* element)
{
     if (list->len == 0)
    {
        list->first = calloc(sizeof(struct sNode), 1);
        list->first->next = NULL;
        list->first->content = element;
        list->len++;
    }
    else
    {
        struct sNode* pointer = list->first;
        while(pointer->next)
            pointer = pointer->next;
        pointer->next = calloc(sizeof(struct sNode), 1);
        pointer->next->next = NULL;
        pointer->next->content = element;
        list->len++;
    }
}

void* sList_pop(struct sList* list, int index)
{
    void* res;
    if(list->len == 0 || index < 1)
    {
        printf("eLists: nothing to pop. No elements in the list or the index is lower than 1!\n");
        return NULL;
    }
    else if(list->len == 1 && index == 1)
    {
        res = list->first->content;
        free(list->first);
        list->len--;
        return res;
    }
    else if(list->len == 1 && index > 1)
    {
        printf("eLists: are you crazy? The index is higher than the length!!!\n");
        return NULL;
    }
    else if(index == 1) //levoe granichnoe - delete second, not first element, and readress next of first
    {
        res = list->first->content;
        struct sNode* pointer1 = list->first;
        struct sNode* pointer2 = list->first->next;
        pointer1->content = pointer2->content;
        pointer1->next = pointer2->next;
        free(pointer2);
        list->len--;
        return res;
    }
    else if(index == list->len) //pravoe granichnoe - delete last and nullify next of prelast
    {
        struct sNode* pointer1 = list->first;
        for(int i = 1; i < list->len - 1; i++)
        {
            pointer1 = pointer1->next;
        }
        struct sNode* pointer2 = pointer1->next;
        res = pointer2->content;
        free(pointer2);
        pointer1->next = NULL;
        list->len--;
        return res;
    }
    else
    {
        struct sNode* pointer1 = list->first;
        for(int i = 1; i < index - 1; i++)
        {
            pointer1 = pointer1->next;
        }
        struct sNode* pointer2 = pointer1->next;
        res = pointer2->content;
        pointer1->next = pointer2->next;
        free(pointer2);
        list->len--;
        return res;
    }
    return NULL;
}

void* sList_get(struct sList* list, int i)
{
    if(list->len < i) return NULL;
    else
    {
        struct sNode* pointer = list->first;
        for(unsigned j = 1; j < i; j++)
            pointer = pointer->next;
        return pointer->content;
    }
}


//  iterator things
//  beware of editing list and simoultenousely using this boyssss - it will crack your program...
//  use for(int i = 1; ...)... ->get(...) insted
void* sList_begin(struct sList* self)
{
    self->_iterator = self->first;
    return self->_iterator->content;
}

void* sList_next(struct sList* self)
{
    self->_iterator = self->_iterator->next;
    return self->_iterator->content;
}

int sList_done(struct sList* self)
{
    return self->_iterator->next == NULL ? 1 : 0;
}