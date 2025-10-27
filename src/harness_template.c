#include <stdio.h>
void bad(void);
void good(void);
int main(void) {
  /* Minimal harness; your agent will evolve this to feed inputs/evidence. */
  bad();
  return 0;
}