/*--
File: timer.h
Description: This file contains definitions about structures 
  and time functions to 'luatimer' package
Date: 2003, November
Author: Silvana Rossetto, silvana at ic.uff.br
Maintainer: Bruno Silvestre, brunoos at inf.puc-rio.br
--*/

#ifndef TIMER_H
#define TIMER_H

/* Struct of a TimerCell */
typedef struct Cell {
   double frequency;
   double expires;
   struct Cell* next;
} TimerCell;

/* List of TimerCells */
typedef struct {
   TimerCell* head;
   TimerCell* tail;
   double earliest;
} TimerList;


/* Time functions */
/* register a timer with a frequency */
void* insertTimer(double frequency);

/* remove a timer */
int removeTimer(void *timer);

/* return the first timer found in list that expiry its time */
void* luatimer_timeout();

/* return the time of the next expiry */
double get_expiry(); 

/* return the time now */
double get_now();

#endif
