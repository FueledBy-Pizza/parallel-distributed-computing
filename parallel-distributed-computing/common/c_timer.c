//
//  c_timer.c
//  parallel-distributed-computing
//
//  Created by Giuliano Aiello on 11/10/24.
//

#include "c_timer.h"
#include <sys/time.h>

/**
 * The difference between two calls gives the execution time of a section of code.
 *
 * Credits: prof. Marco Lapegna
 *
 * @return current system time in seconds
 */
double get_cur_time() {
  struct timeval   tv;
  struct timezone  tz;
  double cur_time;
  
  gettimeofday(&tv, &tz);
  cur_time = tv.tv_sec + tv.tv_usec / 1000000.0;
  
  return cur_time;
}
