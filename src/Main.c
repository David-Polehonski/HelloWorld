#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include <string.h>

#include "uci/UCI.h"

#ifdef __MINGW32__
#include <fcntl.h>
#define pipe(fds) _pipe(fds, 1024, _O_BINARY)
#endif

#ifdef __WIN32__
#define lastError() WSAGetLastError()
#else
#define lastError() errno
#endif

static volatile bool __running__ = true;
static volatile int __exit_code__ = 0;

static const int _BUFFER_SIZE_ = 256;

static int ipcIn;
static int ipcOut;

static FILE *ipcWrite;

static void appendCharacter(char **strPtr, char chr)
{
	**strPtr = chr;	 //	Pointer to the write position(pointer) of the string
	(*strPtr)++;		 //	Order precedence matters, dereference the write position pointer, then increment.
	**strPtr = '\0'; // Terminate string?
}

void signalHandler(int signum)
{
	fprintf(ipcWrite, "Signal Recieved:%i\n", signum);
	fflush(ipcWrite);
	__running__ = false;
	__exit_code__ = signum;
}

void *uci_thread(void *args)
{
	int *id = (int *)args;
	char buffer[_BUFFER_SIZE_];
	char *pos = buffer;

	while (1)
	{
		char c = fgetc(stdin);
		if (c == EOF || c == '\n')
		{
			char *response = uciExecute(buffer);
			if (strlen(response) == 0)
			{
				fprintf(ipcWrite, "%s", "quitting...\n");
				fflush(ipcWrite);
				__running__ = false;
				pthread_exit(0);
			}
			else
			{
				fprintf(ipcWrite, "%s", response);
				fflush(ipcWrite);
			}
			//	Reset buffer
			memset(buffer, '\0', sizeof(buffer));
			pos = buffer;
		}
		else
		{
			appendCharacter(&pos, c);
			if (strlen(buffer) == _BUFFER_SIZE_)
			{
				pos = buffer;
			}
		}
	}
}

struct listener
{
	unsigned int *flagset;
	unsigned int flagmask;
	FILE *stream;
};
typedef struct listener listener_t;

void *peek(void *args)
{
	int c;
	listener_t *l = (listener_t *)args;

	FILE *stream = l->stream;

	c = fgetc(stream);
	ungetc(c, stream);
	*l->flagset = (*l->flagset) ^ l->flagmask;
}

unsigned int listen(FILE *stream)
{
	unsigned int flagset = 0;

	listener_t *args = (listener_t *)malloc(sizeof(listener_t));
	args->flagset = &flagset;
	args->flagmask = 1;
	args->stream = stream;

	pthread_t threadId;
	int i = pthread_create(&threadId, NULL, peek, args);
	if (i != 0)
	{
		exit(i);
	}
	do
	{
		sleep(0.1);
	} while (flagset == 0);

	free(args);
	return flagset;
}

int main(void)
{
	signal(SIGINT, signalHandler);

	/* Create the pipe. */
	int internalPipe[2];
	if (pipe(internalPipe))
	{
		return EXIT_FAILURE;
	}

	ipcIn = internalPipe[0];
	ipcOut = internalPipe[1];

	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	pthread_t uci_id;
	pthread_create(&uci_id, NULL, uci_thread, NULL);

	FILE *stream = fdopen(ipcIn, "r");
	ipcWrite = fdopen(ipcOut, "w");
	do
	{
		unsigned int result = listen(stream);
		if (result > 0)
		{
			char buffer[_BUFFER_SIZE_];
			int pos = 0;
			do
			{
				buffer[pos] = getc(stream);
			} while (buffer[pos++] != '\n' && pos < _BUFFER_SIZE_);
			buffer[pos] = '\0';
			printf("%s", buffer);
		}
	} while (__running__);

	pthread_exit(NULL);

	return __exit_code__;
}
