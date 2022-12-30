#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>


static void handler(int signum)
{
	extern volatile sig_atomic_t last_received_signal;
	last_received_signal = (sig_atomic_t) signum;
}

static void ( *xsignal(int signum, void (*handler)(int)) )(int)
{
	struct sigaction sa, oldsa;
	sa.sa_handler = handler;
	sa.sa_flags = 0;
	sigfillset(&sa.sa_mask);
	if (sigaction(signum, &sa, &oldsa) == 0)
		return oldsa.sa_handler;
	return SIG_ERR;
}

int main(int argc, char **argv)
{
	extern volatile sig_atomic_t last_received_signal;
	unsigned int timeout, period;
	void (*result)(int);
	int i, signum, count, signal_list[] = { SIGALRM, SIGUSR1, SIGUSR2, SIGINT, SIGTERM };

	fprintf(stderr, "Starting: %d...\n", (int)getpid());

	period = argc > 1 ? strtol(*(argv + 1), NULL, 0) : 5;
	fprintf(stderr, "Alarm period set to: %u\n", period);

	count = sizeof signal_list / sizeof signal_list[0];
	for (i = 0; i < count; ++i) {
		signum = signal_list[i];
		result = xsignal(signum, handler);
		if (result == SIG_ERR) {
			fprintf(stderr, "Oops! Error setting handler for signal: %d\n", signum);
			exit(EXIT_FAILURE);
		}
		fprintf(stderr, "Set handler for signal: %d\n", signum);
	}

	fprintf(stderr, "Total of %d handlers set!\n", count);

	for (;;) {
		timeout = alarm(period);
		if (timeout > 0) {
			fprintf(stderr, "Alarm canceled %d seconds before due...\n", timeout);
		}
		pause();
		signum = (int) last_received_signal;
		last_received_signal = 0;
		fprintf(stderr, "Last signal received: %d\n", signum);
		if (signum == SIGTERM) {
			fprintf(stderr, "Good Bye!\n");
			break;
		}
	}

	return EXIT_SUCCESS;
}

volatile sig_atomic_t last_received_signal = 0;
