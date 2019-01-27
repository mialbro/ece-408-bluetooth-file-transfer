/*
    Author: Mark Douglas Ray Robinson
    Date: September 14, 2018
    Title: Assignment 0
    Description: Circular dynamic array which is used to implement the queue data structure
*/

#ifndef __CDA_INCLUDED__
#define __CDA_INCLUDED__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef struct cda CDA;

extern CDA *newCDA(void);
extern void setCDAdisplay(CDA *items, void (*display)(void *,FILE *));
extern void setCDAfree(CDA *items, void (*free)(void *));
extern void insertCDA(CDA *items,int index,void *value);
extern void *removeCDA(CDA *items,int index);
extern void unionCDA(CDA *recipient,CDA *donor);
extern void *getCDA(CDA *items,int index);
extern void *setCDA(CDA *items,int index,void *value);
extern int  sizeCDA(CDA *items);
extern void displayCDA(CDA *,FILE *);
extern int  debugCDA(CDA *,int level);
extern void freeCDA(CDA *);

#define insertCDAfront(items,value) insertCDA(items,0,value)
#define insertCDAback(items,value)  insertCDA(items,sizeCDA(items),value)
#define removeCDAfront(items)       removeCDA(items,0)
#define removeCDAback(items)        removeCDA(items,sizeCDA(items)-1)

#endif
