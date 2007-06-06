/*--
File: timerlib.c
Description: This file contains the time functions to package 'luatimer'
Date: 2003, November
Version: 1.0
Comments to: silvana@inf.puc-rio.br
--*/

#include <sys/time.h>
#ifndef __linux__
#include <sys/limits.h>
#else
#include <values.h>
#endif /* Linux is always conformant... */

#include <stdlib.h>

#include "timer.h"

#define SLEEP_MAX  60*60*24 


/* Start the TimerCell's list */
//static TimerList tlist = {NULL, NULL, SLEEP_MAX};
static TimerList tlist = {NULL, NULL, DBL_MAX};


/* Auxiliary functions to insert and remove TimerCells in list */
/* Update the 'tlist.earliest' value */
static void _upEarliest() {
    double earliest = DBL_MAX;

    TimerCell* tobj = tlist.head;
    while (tobj) {
       earliest = (tobj->expires < earliest) ? tobj->expires : earliest;
       tobj = tobj->next;
    }
    tlist.earliest = earliest;
}

/* Insert a object in the list 'tlist' */
static void _insert(TimerCell* tobj) {
    // set expiration time
    tobj->expires = get_now() + tobj->frequency;
    tobj->next = NULL;

    // case empty list
    if (tlist.head == NULL) {
       tlist.head = tlist.tail = tobj;
       tlist.earliest = tobj->expires;
    }
    else { // case no empty list
       TimerCell* last = tlist.tail;
       last->next = tobj;
       tlist.tail = tobj;
       tlist.earliest = 
	(tobj->expires < tlist.earliest) ? tobj->expires : tlist.earliest;
    }
}

/* Remove a object from the list 'tlist' */
static int _remove(TimerCell* tobj) {
   if (tobj == tlist.head) {
      if (tobj == tlist.tail) {
         tlist.head = tlist.tail = NULL;
         tlist.earliest = SLEEP_MAX;
         return 1;
      }
      else {
         tlist.head = tobj->next;
         if (tobj->expires == tlist.earliest) {
            // update the 'tlist.earliest' value 
            _upEarliest(); 
         }
         return 1;
      }
   } else {
      TimerCell* prev = tlist.head;
      while (prev) {
        if (prev->next == tobj) {
           prev->next = tobj->next;
           if (prev->next == NULL) tlist.tail = prev;
           if (tobj->expires == tlist.earliest) {
              // update the 'tlist.earliest' value 
              _upEarliest(); 
           }
           return 1;
        }
        prev = prev->next;
      }
   }
   // the object does not found
   return 0;
}


/*-------------------------- API LuaTimer -------------------------*/
/* Create a new Timer */
void* insertTimer(double frequency) {
   TimerCell* cell = (TimerCell*) malloc(sizeof(TimerCell));
   cell->frequency = frequency;
   _insert(cell);

   return (void*) cell;
}

/* Remove a Timer */
int removeTimer(void* cell) {
   int ret =_remove((TimerCell*) cell);
   if (ret) {
      free((TimerCell*) cell);
   }
   return ret;
}

/* Return the first timer expired */
void* luatimer_timeout() {
    double now, rest;
    TimerCell *tobj;
    
    now = get_now();
    tobj = tlist.head;
    while (tobj) {
       rest = tobj->expires - now;
       if (tobj->expires <= now) { 
          tobj->expires = now + (tobj->frequency);
	  _upEarliest();
	  return (void*) tobj;
       }
       tobj = tobj->next;
    }
    return NULL;
}

/* Return the time now */
double get_now() {
    struct timeval tvnow;
    double now;

    gettimeofday(&tvnow, (struct timezone *)0);
    now = tvnow.tv_sec + ((double)tvnow.tv_usec)/1000000L;
   
    return now;
}

/* Return the next expires time */
double get_expiry() {
   double now = get_now();
   return tlist.earliest - now;
}
