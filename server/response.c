#include "response.h"

void get_time(char *timeString, int maxSize) {

  struct tm *tm;
  time_t t;

  t = time(NULL);
  tm = localtime(&t);

  strftime(timeString, maxSize, "%Y.%m.%d %H:%M:%S", tm);
}
