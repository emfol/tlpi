#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	char **path, *paths[] = { "/dev/random", "/dev/urandom", "/dev/arandom", NULL };
	int r_fd, r_val;
	ssize_t count;
	for (path = paths; *path != NULL; ++path) {
		r_fd = open(*path, O_RDONLY | O_NONBLOCK, 0);
		if (r_fd < 0) {
			fprintf(stderr, "Error opening file \"%s\": %s\n", *path, strerror(errno));
			continue;
		}
		count = read(r_fd, &r_val, sizeof r_val);
		if (count != (ssize_t)(sizeof r_val)) {
			if (count < 0)
				fprintf(stderr, "Error reading from \"%s\": %s\n", *path, strerror(errno));
			else
				fprintf(stderr, "Partial read from \"%s\"...\n", *path);
		} else {
			fprintf(stderr, "Random number from \"%s\": ", *path);
			printf("%08d\n", r_val);
		}
		close(r_fd);
	}
	fputs("Done!\n", stderr);
	return EXIT_SUCCESS;
}
