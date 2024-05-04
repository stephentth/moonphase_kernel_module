#include <stdio.h>

#include "moon.h"

int main(void) {
  int result;

  result = moon_phase(2000, 1, 1);
  if (result != 7) {
    printf("got %d expect %d\n", result, 1);
    return 1;
  }

  return 0;
}