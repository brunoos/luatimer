/*
Author: Silvana Rossetto, silvana at ic.uff.br
Maintainer: Bruno Silvestre, brunoos at inf.puc-rio.br
*/

#ifndef __LIBTIMER_H__
#define __LIBTIMER_H__

/**
 * Struct of a TimerCell 
 */
typedef struct Cell {
  double frequency;
  double expires;
  struct Cell* next;
} TimerCell;

/**
 *  List of TimerCells.
 */
typedef struct {
  double earliest;
  TimerCell* head;
  TimerCell* tail;
} TimerList;

typedef struct {
  double now;
  TimerCell *curr;
} Iterator;


/* Return the seconds for the next expiration. */
double libt_nextshot(); 

/* Return the time now -- in seconds since Epoch. */
double libt_now();

/* Add the timer. */
void libt_add(TimerCell *timer, double frequency);

/* Remove the timer. */
int libt_remove(TimerCell *timer);

/* Create a new iterator */
Iterator* libt_newiterator();

/* Finalize the iterator */
void libt_freeiterator(Iterator *iter);

/* Return the first fired timer according the iterator position */
TimerCell* libt_nextfired(Iterator *iter);

#endif
