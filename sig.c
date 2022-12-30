#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define DEFAULT_ALARM_PERIOD 5
#define SIGNAL_LIST_ENTRY(signame) { (signame), 0, #signame }

struct signal_list_entry {
	int num;
	unsigned int count;
	char *name;
};

static void handler(int signum)
{
	extern volatile sig_atomic_t last_received_signal;
	last_received_signal = (sig_atomic_t) signum;
}

static void ( *x_signal(int signum, void (*handler)(int)) )(int)
{
	struct sigaction sa, oldsa;
	sa.sa_handler = handler;
	sa.sa_flags = 0;
	sigfillset(&sa.sa_mask);
	if (sigaction(signum, &sa, &oldsa) == 0)
		return oldsa.sa_handler;
	return SIG_ERR;
}

static struct signal_list_entry *find_signal_list_entry(
	struct signal_list_entry *entry,
	int count,
	int num
)
{
	struct signal_list_entry *limit;
	if (entry != NULL && count > 0)
		for (limit = entry + count; entry < limit; ++entry)
			if (entry->num == num)
				return entry;
	return NULL;
}

int main(int argc, char **argv)
{
	extern volatile sig_atomic_t last_received_signal;
	void (*result)(int);
	int i, count, signum;
	unsigned int timeout, period;
	struct signal_list_entry *entry, signal_list[] = {
		SIGNAL_LIST_ENTRY(SIGINT),
		SIGNAL_LIST_ENTRY(SIGQUIT),
		SIGNAL_LIST_ENTRY(SIGUSR1),
		SIGNAL_LIST_ENTRY(SIGUSR2),
		SIGNAL_LIST_ENTRY(SIGALRM),
		SIGNAL_LIST_ENTRY(SIGTERM)
	};

	fprintf(
		stderr,
		"Starting...\n"
		" - PID: %d (child of %d)\n"
		" - Size of \"int\": %lu\n"
		" - Size of \"long int\": %lu\n"
		" - Size of \"sig_atomic_t\": %lu\n"
		" - Size of \"sigset_t\": %lu\n",
		(int)getpid(),
		(int)getppid(),
		sizeof(int),
		sizeof(long int),
		sizeof(sig_atomic_t),
		sizeof(sigset_t)
	);

	/* Determine alarm period... */
	period = argc > 1 ? strtol(*(argv + 1), NULL, 0) : DEFAULT_ALARM_PERIOD;
	fprintf(stderr, " - Alarm period set to: %u second(s)\n", period);

	count = (int)(sizeof signal_list / sizeof signal_list[0]);
	for (i = 0; i < count; ++i) {
		entry = &signal_list[i]; /* signal_list + i */
		result = x_signal(entry->num, handler);
		if (result == SIG_ERR) {
			fprintf(
				stderr,
				"[ERROR] Oops... Could not set handler for signal: %s (%d)\n",
				entry->name,
				entry->num
			);
			exit(EXIT_FAILURE);
		}
		fprintf(
			stderr,
			" - Set handler for signal: %s (%d)\n",
			entry->name,
			entry->num
		);
	}

	fprintf(stderr, " - Total of %d handlers set!\n", count);

	for (;;) {
		timeout = alarm(period);
		if (timeout > 0) {
			fprintf(stderr, " - Alarm canceled %d seconds before due...\n", timeout);
		}
		pause();
		signum = (int)last_received_signal;
		last_received_signal = 0;
		entry = find_signal_list_entry(signal_list, count, signum);
		if (entry != NULL) {
			if (entry->num != signum) {
				fprintf(
					stderr,
					"[ERROR] Unexpected mismatch on signal numbers: %d != %d\n",
					entry->num,
					signum
				);
				abort();
			}
			++entry->count;
			fprintf(
				stderr,
				" - Last signal received: %s (#%d x %u)\n",
				entry->name,
				entry->num,
				entry->count
			);
			if ((signum == SIGTERM || signum == SIGINT) && entry->count >= 3)
				break;
		} else {
			fprintf(stderr, " - Last signal received: %d (unknown)\n", signum);
			if (signum == SIGTERM)
				break;
		}
	}

	fprintf(stderr, "Good Bye!\n");
	return EXIT_SUCCESS;
}

volatile sig_atomic_t last_received_signal = 0;
