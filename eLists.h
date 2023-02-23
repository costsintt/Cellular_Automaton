#ifndef ELISTS
#define ELISTS

#include <stdlib.h>
#include <stdio.h>

enum {ELISTS_SIMPLE, ELISTS_SIMPLE_ARRAY};

struct sNode
{
    void* content;
    struct sNode* next;
};

struct sList
{
    int len;
    struct sNode* first;
    struct sNode* _iterator;

    void*(*pop)(struct sList*, int);
    void(*push)(struct sList*, void*);
    void*(*get)(struct sList*, int);
    
    //iteration shits
    void*(*begin)(struct sList* self);
    void*(*next)(struct sList* self);
    int(*done)(struct sList* self);
};

struct sList* sList_cons();
void sList_decons(struct sList**);

void* sList_get(struct sList*, int); //starting from 1, not 0. first element on 1, not 0
void* sList_pop(struct sList*, int);
void sList_push(struct sList*, void*);

void* sList_begin(struct sList* self);
void* sList_next(struct sList* self);
int sList_done(struct sList* self);

#endif