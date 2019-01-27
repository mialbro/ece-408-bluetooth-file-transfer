/*
    Author: Mark Douglas Ray Robinson
    Date: September 14, 2018
    Title: Assignment 0
    Description: Dynamic array which is used to implement the stack data structure
*/

#include "da.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

struct da {
    void (*display)(void *, FILE *);
    void (*free)(void *);
    int size;
    int capacity;
    int debugLevel;
    void **store;
};

//  Generates a new dynamic array and assigns each field to their default values
DA *newDA(void) {
    DA *n = malloc(sizeof(DA));
    assert(n != 0);
    n->store = malloc(sizeof(void *));
    assert(n->store != 0);
    n->size = 0;
    n->capacity = 1;
    n->display = 0;
    n->free = 0;
    n->debugLevel = 0;
    return n;
}

/*  Helper Functions  */

// Shifts array when an element is inerted
static void insertShift(DA *items, int index, int prevIndex) {
  for (int i = prevIndex; i > index; i--)
    setDA(items, i, getDA(items, i - 1));
}

// Shifts array when an element is removed
static void removeShift(DA *items, int index) {
  for (int i = index; i < sizeDA(items) - 1; i++) {
    setDA(items, i, getDA(items, i + 1));
  }
}

static void shrink(DA *items) {
  if (sizeDA(items) == 0)
    items->capacity = 1;
  else
    items->capacity /= 2;
  items->store = realloc(items->store, items->capacity * sizeof(void *));
  assert(items->store != 0);
}

/**** Public Interface ****/


// Updates the display method pointer
void setDAdisplay(DA *items,void (*display)(void *,FILE *)) {
  items->display = display;
}

// Updates the free method pointer
void setDAfree(DA *items,void (*free)(void *)) {
  items->free = free;
}

// Inserts item into dynamic array increases size of array if necessary
void insertDA(DA *items,int index,void *value) {
  assert(index >= 0 && index <= sizeDA(items));
  if (sizeDA(items) == items->capacity) {
    items->capacity *= 2;
    items->store = realloc(items->store, items->capacity * sizeof(void *));
    assert(items->store != 0);
  }

  int prevIndex = sizeDA(items);
  items->size += 1;

  if (index < sizeDA(items) - 1)
    insertShift(items, index, prevIndex);

  setDA(items, index, value);
}

// Removes item from array, shrinks array if necessary
void *removeDA(DA *items,int index) {
  assert(sizeDA(items) > 0);
  void *temp = getDA(items, index);

  removeShift(items, index);

  items->size -= 1;

  if ((float)(sizeDA(items)) / (items->capacity) < 0.25 && (items->capacity) / 2 >= 1)
    shrink(items);
  return temp;
}

// Appends the items in donor to recipient array
void unionDA(DA *recipient,DA *donor) {
  for (int i = 0; i < sizeDA(donor); i++)
    insertDA(recipient, sizeDA(recipient), getDA(donor, i));

  free(donor->store);
  donor->size = 0;
  donor->capacity = 1;
  donor->store = 0;
  donor->store = malloc(sizeof(void *));
}

// Returns item at specified index
void *getDA(DA *items,int index) {
  assert(index >= 0 && index < sizeDA(items));
  return items->store[index];
}

// Sets item at specified index to the provided value
void *setDA(DA *items,int index,void *value) {
  assert(index >= 0 && index <= sizeDA(items));
  if (index == sizeDA(items)) {
    insertDAback(items, value);
    return NULL;
  }
  void *temp = getDA(items, index);
  items->store[index] = value;
  return temp;
}

// Returns the number of items in the array (also next index for insert-to-back)
int sizeDA(DA *items) {
  return items->size;
}

// Displays the items in the array
void displayDA(DA *items,FILE *fp) {
  fprintf(fp, "[");
  for (int i = 0; i < sizeDA(items); i++) {
    if (items->display)
      items->display(items->store[i], fp);
    else
      fprintf(fp, "@%p", (items->store[i]));
    if (i < sizeDA(items) - 1)
      fprintf(fp, ",");
  }
  if (items->debugLevel > 0) {
    if (sizeDA(items) > 0)
      fprintf(fp, ",");
    fprintf(fp, "[%d]", items->capacity - sizeDA(items));
  }
  fprintf(fp, "]");
}

// Sets a new debug level and returns the previous one
int debugDA(DA *items,int level) {
  int prevLevel = items->debugLevel;
  items->debugLevel = level;
  return prevLevel;
}

// frees the memory allocated for the specified structure
void freeDA(DA *items) {
  if (items->free) {
  for (int i = 0; i < sizeDA(items); i++)
    items->free(getDA(items, i));
  }
  free(items->store);
  free(items);
}
