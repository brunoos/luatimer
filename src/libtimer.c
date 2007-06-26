/*
Author: Silvana Rossetto, silvana at ic.uff.br
Maintainer: Bruno Silvestre, brunoos at inf.puc-rio.br
*/

#include <sys/time.h>
#include <time.h>
#include <float.h>
#include <stdlib.h>

#include "libtimer.h"


/**
 * Initialize the TimerCell's list.
 */
static TimerList tlist = {DBL_MAX, NULL, NULL};


/**
 * Update the value of the the earliest timer.
 */
static void libt_update() 
{
  double earliest = DBL_MAX;

  TimerCell* tobj = tlist.head;
  while (tobj) {
    if (tobj->expires < earliest)
      earliest = tobj->expires;
    tobj = tobj->next;
  }
  tlist.earliest = earliest;
}

/**
 * Return the time now -- seconds since Epoch.
 */
double libt_now() 
{
  struct timeval tvnow;
  gettimeofday(&tvnow, NULL);
  return (double)tvnow.tv_sec + ((double)tvnow.tv_usec/1e6f);
}

/**
 * Return the seconds for the next expiration.
 */
double libt_nextshot() 
{
  double next = tlist.earliest - libt_now();
  return (next > 0) ? next : 0;
}

/**
 * Add a timer in the list.
 */
void libt_add(TimerCell* tobj, double frequency) 
{
  /* Set expiration time */
  tobj->frequency = frequency;
  tobj->expires = frequency + libt_now();
  tobj->next = NULL;

  if (tlist.head == NULL) {
    tlist.head = tobj;
    tlist.tail = tobj;
    tlist.earliest = tobj->expires;
  }
  else { 
    TimerCell* last = tlist.tail;
    last->next = tobj;
    tlist.tail = tobj;
    if (tobj->expires < tlist.earliest)
      tlist.earliest = tobj->expires;
  }
}

/**
 * Remove a timer from the list.
 */
int libt_remove(TimerCell* tobj) 
{
  if (tobj == tlist.head) {
    if (tobj == tlist.tail) {
      tlist.head = NULL;
      tlist.tail = NULL;
      tlist.earliest = DBL_MAX;
    }
    else {
      tlist.head = tobj->next;
      if (tobj->expires == tlist.earliest)
        libt_update(); 
    }
    return 1;
  } 
  else {
    TimerCell* prev = tlist.head;
    while (prev) {
      if (prev->next == tobj) {
        prev->next = tobj->next;
        if (prev->next == NULL) 
          tlist.tail = prev;
        if (tobj->expires == tlist.earliest)
          libt_update(); 
        return 1;
      }
      prev = prev->next;
    }
  }
  /* The object does not exist */
  return 0;
}

Iterator* libt_newiterator()
{
  Iterator* iter = (Iterator*) malloc(sizeof(Iterator));
  if (iter) {
    iter->now = libt_now();
    iter->curr = tlist.head;
  }
  return iter;
}

void libt_freeiterator(Iterator *iter)
{
  if (iter) {
    free(iter);
    iter = NULL;
    libt_update();
  }
}

TimerCell* libt_nextfired(Iterator *iter)
{
  if (iter) {
    TimerCell *curr = iter->curr;
    while (curr) {
      if (curr->expires <= iter->now) { 
        curr->expires = iter->now + curr->frequency;
        iter->curr = curr->next;
        return curr;
      }
      curr = curr->next;
    }
  }
  return NULL;
}
