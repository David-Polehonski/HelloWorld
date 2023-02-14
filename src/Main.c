/*
 ============================================================================
 Name        : HelloWorld.c
 Author      :
 Version     :
 Copyright   :
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#include <string.h>

#include "Position.h"
#include "UCI.h"

static volatile bool __running__ = true;
static volatile int exit_code = 0;
static const int _BUFFER_SIZE_ = 256;

static void appendCharacter(char **strPtr, char chr)
{
	**strPtr = chr;	 //	Pointer to the write position(pointer) of the string
	(*strPtr)++;		 //	Order precedence matters, dereference the write position pointer, then increment.
	**strPtr = '\0'; // Terminate string?
}

void *engine(void *args)
{
	int *id = (int *)args;
}

int main(void)
{
	char buffer[_BUFFER_SIZE_];
	char *pos = buffer;

	pthread_t engine_id;
	pthread_create(&engine_id, NULL, engine, NULL);

	setbuf(stdout, NULL);
	do
	{
		char c = fgetc(stdin);

		if (c == EOF || c == '\n')
		{
			char *token = buffer;
			strtok(token, " ");

			char *response = uciExecute(token);
			if (strlen(response) == 0)
			{
				__running__ = false;
				exit_code = 0;
			}
			else
			{
				printf(response);
			}
			//	Reset buffer
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
	} while (__running__);

	pthread_exit(NULL);

	return exit_code;
}
