/*
    Author: Mark Douglas Ray Robinson
    Date: September 14, 2018
    Title: Assignment 0
    Description: Uses the dynamic array module to implement the stack data structure
*/

#include "da.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

struct stack {
    DA *da;
    void (*display)(void *, FILE *);
    int debugLevel;
};

// Returns initialized stack structure
STACK *newSTACK(void) {
    STACK *n = malloc(sizeof(STACK));
    assert(n != 0);
    n->da = newDA();
    n->display = 0;
    n->debugLevel = 0;
    return n;
}

// Sets the stack's display method
void setSTACKdisplay(STACK *items,void (*display)(void *,FILE *)) {
  items->display = display;
  setDAdisplay(items->da, display);
}

// Sets the stack's free method
void setSTACKfree(STACK *items,void (*free)(void *)) {
  setDAfree(items->da, free);
}

// Adds item to back of dynamic array
void push(STACK *items,void *value) {
  insertDAback(items->da, value);
}

// Removes item from back of dynamic array
void *pop(STACK *items) {
  assert(sizeSTACK(items) > 0);
  return removeDAback(items->da);
}

// Shows next item to be popped (from back)
void *peekSTACK(STACK *items) {
  assert(sizeSTACK(items) > 0);
  return getDA(items->da, sizeSTACK(items) - 1 );
}

// Displays the items in the stack
void displaySTACK(STACK *items,FILE *fp) {
  if (items->debugLevel == 1 || items->debugLevel == 2) {
    debugDA(items->da, items->debugLevel - 1);
    displayDA(items->da, fp);
  }
  else {
    fprintf(fp, "|");
    for (int i = sizeSTACK(items) - 1; i >= 0; i--) {
      if (items->display)
        items->display(getDA(items->da, i), fp);
      else
        fprintf(fp, "@%p", getDA(items->da, i));
      if (i > 0)
          fprintf(fp, ",");
    }
    fprintf(fp, "|");
  }
}

int debugSTACK(STACK *items, int newLevel) {
  int prevLevel = items->debugLevel;
  items->debugLevel = newLevel;
  return prevLevel;
}

// frees the memory allocated for the specified structure
void freeSTACK(STACK *items) {
  freeDA(items->da);
  free(items);
}

// return size of array
int sizeSTACK(STACK *items) {
  return sizeDA(items->da);
}
