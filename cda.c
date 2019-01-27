/*
    Author: Mark Douglas Ray Robinson
    Date: September 14, 2018
    Title: Assignment 0
    Description: Circular dynamic array which is used to implement the queue data structure
*/

#include "cda.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

struct cda {
    void (*display)(void *, FILE *);  // setDAdisplay
    void (*free)(void *);  // setDAfree
    int size;
    int capacity;
    int debugLevel;
    void **store;
    int sIndex;
};

// Returns the correct index
static int getCorrectIndex(CDA *items, int index) {
    return (index + items->sIndex + items->capacity) % items->capacity;
}

// Returns a cda struct with the default values
CDA *newCDA(void) {
  CDA *n = malloc(sizeof(CDA));
  assert(n != 0);
  n->store = malloc(sizeof(void *));
  assert(n->store != 0);
  n->size = 0;
  n->capacity = 1;
  n->display = 0;
  n->free = 0;
  n->debugLevel = 0;
  n->sIndex = 0;
  return n;
}

        /*  Helper Functions  */

// Grows the array
static void grow(CDA *items) {
  void **tempArr = malloc(2 * items->capacity * sizeof(void *));
  assert(tempArr != 0);
  for (int i = 0; i < sizeCDA(items); i++) {
    tempArr[i] = getCDA(items, i);
  }
  free(items->store);
  items->store = tempArr;
  items->capacity *= 2;
  items->sIndex = 0;
}

// Shrinks the array
static void shrink(CDA *items) {
  int newCap = items->capacity / 2;

  if (sizeCDA(items) == 0)
    newCap = 1;

  void **tempArr = malloc(items->capacity * sizeof(void *));
  assert(tempArr != 0);
  for (int i = 0; i < sizeCDA(items); i++)
    tempArr[i] = getCDA(items, i);
  free(items->store);
  items->store = tempArr;
  items->capacity = newCap;
  items->sIndex = 0;
}

// Shifts array when value inserted
static void insertShift(CDA *items, int index, int prevSize) {
  if ((index)  < (prevSize / 2)) {
    items->sIndex = (items->sIndex - 1 + items->capacity) % items->capacity;
    for (int i = 0; i < index; i++)
      setCDA(items, i, getCDA(items, i + 1));
  }
  else {
    for (int i = prevSize; i > index; i--)
      setCDA(items, i, getCDA(items, i - 1));
  }
}

// Shifts array when a value is removed
static void removeShift(CDA *items, int index) {
  if ((sizeCDA(items) / 2) < index) {
    for (int i = index; i < sizeCDA(items) - 1; i++)
      setCDA(items, i, getCDA(items, i + 1));
    return;
  }
  for (int i = index; i > 0; i--)
    setCDA(items, i, getCDA(items, i - 1));
  items->sIndex = (items->sIndex + 1 + items->capacity) % items->capacity;
}

// Sets the display pointer to a display method
void setCDAdisplay(CDA *items, void (*display)(void *,FILE *)) {
  items->display = display;
}

// Sets the free pointer to a free method
void setCDAfree(CDA *items, void (*free)(void *)) {
  items->free = free;
}

// Inserts item into the circular dynamic array, grows if necessary
void insertCDA(CDA *items,int index,void *value) {
  assert(index >= 0 && index <= sizeCDA(items));
  if (sizeCDA(items) == items->capacity) {
    grow(items);
  }
  // Add To Front
  if (index == 0) {
    if (sizeCDA(items) == 0) {
      items->store[0] = value;
    }
    else {
      items->sIndex = (items->sIndex - 1 + items->capacity) % items->capacity;
      items->store[items->sIndex] = value;
    }
    items->size += 1;
    return;
  }

  if (index == sizeCDA(items)) {
    items->store[getCorrectIndex(items, sizeCDA(items))] = value;
    items->size += 1;
    return;
  }

  int prevSize = sizeCDA(items);
  items->size += 1;

  insertShift(items, index, prevSize);
  setCDA(items, index, value);
  return;
}

// removes item from array, shrinks array if necessary
void *removeCDA(CDA *items, int index) {
  assert(index >= 0 && index < sizeCDA(items));
  assert(sizeCDA(items) > 0);
  void *removedItem = getCDA(items, index);

  if (index == 0)
    items->sIndex = (items->sIndex + 1 + items->capacity) % items->capacity;
  else
    removeShift(items, index);

  items->size -= 1;

  if ((float)(sizeCDA(items)) / (items->capacity) < 0.25 && (float)(items->capacity) / 2 >= 1)
    shrink(items);

  return removedItem;
}

// Appends the items in donor to recipient array
void unionCDA(CDA *recipient,CDA *donor) {
  for (int i = 0; i < sizeCDA(donor); i++)
    insertCDA(recipient, sizeCDA(recipient), getCDA(donor, i));

  free(donor->store);
  donor->size = 0;
  donor->capacity = 1;
  donor->store = 0;
  donor->store = malloc(sizeof(void *));
}

// Returns specified item
void *getCDA(CDA *items,int index) {
  assert(index >= 0 && index < sizeCDA(items));
  return items->store[getCorrectIndex(items, index)];
}

// Insert item at front or back, replace previous item at index otherwise
void *setCDA(CDA *items,int index,void *value) {
  assert(index >= -1 && index <= sizeCDA(items));
  if (index == sizeCDA(items)) {
    insertCDAback(items, value);
  }
  else if (index == -1) {
    insertCDAfront(items, value);
  }
  else {
    void *RemovedValue = getCDA(items, index);
    items->store[getCorrectIndex(items, index)] = value;
    return RemovedValue;
  }
  return NULL;
}

// Return number of items in circular dynamic array
int sizeCDA(CDA *items) {
  return items->size;
}

// Display the items in the array
void displayCDA(CDA *items,FILE *fp) {
  fprintf(fp, "(");
  for (int i=0; i<sizeCDA(items); i++) {
    if (items->display)
        items->display(getCDA(items, i), fp);
    else
        fprintf(fp, "@%p", getCDA(items, i));
    if (i < sizeCDA(items) - 1)
        fprintf(fp, ",");
  }
  if (items->debugLevel > 0) {
    if (sizeCDA(items) > 0)
      fprintf(fp, ",");
    fprintf(fp, "(%d)", items->capacity - sizeCDA(items));
  }
  fprintf(fp, ")");
}

// Sets a new debug level and returns the previous one
int debugCDA(CDA *items, int newLevel) {
  int prevLevel = items->debugLevel;
  items->debugLevel = newLevel;
  return prevLevel;
}

// frees the memory allocated for the specified structure
void freeCDA(CDA *items) {
  if (items->free) {
    for (int i = 0; i < sizeCDA(items); i++)
      items->free(getCDA(items, i));
  }
  free(items->store);
  free(items);
}
