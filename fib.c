#include <stdlib.h>
#include <stdio.h>

static long int fib(int n)
{
  long int s, p[2] = { 0L, 1L };
  if (n < 0)
    n = 0;
  while (n > 1)
    s = p[0] + p[1], p[0] = p[1], p[1] = s, --n;
  return p[n & 1];
}

int main(int argc, char **argv)
{
  long int result;
  int index = 0, status = 0;

  /* Disable buffering. */
  setvbuf(stdout, NULL, _IONBF, 0L);
  setvbuf(stderr, NULL, _IONBF, 0L);

  if (argc < 2) {
    /* Print usage when no argument is given... */
    fprintf(stderr, "Usage:\n\t%s <fib_seq_index>\n", *argv);
    status = 1;
  } else {
    /* Parse given index. */
    index = atoi(*(argv + 1));
    if (index < 0) {
      fprintf(
        stderr,
        "The sequence index should be an integer greater than or equal to zero... (Given: %d)\n",
        index
      );
      status = 1;
    }
  }

  /* Calculate result. */
  result = fib(index);

  /* Output result. */
  fprintf(stderr, "fib(%d) = ", index);
  fprintf(stdout, "%ld\n", result);

  return status;
}
