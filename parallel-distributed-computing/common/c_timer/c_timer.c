/******************************************************************************
 * Library Name: `c_timer`
 *
 * Overview:
 * This library provides functions for C-based time operations.
 *
 * Version: 1.0.0
 * Author: prof. Marco Lapegna
 * License: MIT
 *
 */

#include "c_timer.h"
#include <sys/time.h>

/**
 * @brief The difference between two calls gives the execution time of a section of code.
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
