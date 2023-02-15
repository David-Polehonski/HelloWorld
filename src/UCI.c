#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "UCI.h"

FILE *fp;
void writeLog(char *logstring)
{
	if (fp == NULL)
	{
		fp = fopen("hw.log", "a");
	}
	if (fp != NULL)
	{
		fprintf(fp, "%s\n", logstring);
		fflush(fp);
	}
}

void closeLog()
{
	if (fp != NULL)
	{
		fclose(fp);
	}
}
/*
	uci
	Tell engine to use the uci (universal chess interface), this will be send once
	as a first command after program boot to tell the engine to switch to uci mode.
	After receiving the uci command the engine must identify itself with the “id”
	commmand and sent the “option” commands to tell the GUI which engine settings
	the engine supports if any. After that the engine should sent “uciok” to
	acknowledge the uci mode. If no uciok is sent within a certain time period,
	the engine task will be killed by the GUI.
*/
char *uci(char *argTokens)
{
	writeLog("[uci]");
	return "id name HelloWorld\nid author David Polehonski\nuciok\n";
}

/*
isready
This is used to synchronize the engine with the GUI. When the GUI has sent a
command or multiple commands that can take some time to complete, this command
can be used to wait for the engine to be ready again or to ping the engine to
find out if it is still alive.
E.g. This should be sent after setting the path to the tablebases as this can
take some time. This command is also required once before the engine is asked
to do any search to wait for the engine to finish initializing.
This command must always be answered with “readyok” and can be sent also when the
engine is calculating in which case the engine should also immediately answer
with “readyok” without stopping the search.

*/
char *isready(char *args)
{
	return "readyok\n";
}

char *quit(char *args)
{
	writeLog("[quit]");
	closeLog();
	return "";
}

static const char *command_names[] = {"uci", "isready", "quit"};
char *(*command_functions[])(char *) = {uci, isready, quit};

char *uciExecute(char *input)
{
	char log[256] = "[IO] - ";
	strcat(log, input);
	writeLog(log);

	char *command = strtok(input, " ");
	char *arguments = strtok(NULL, "\n");

	int i;
	for (i = 0; i < 2; i++)
	{
		if (strcmp(command, command_names[i]) == 0)
		{
			return (*command_functions[i])(arguments);
		}
	}
	return "";
}

/*

debug [ on | off ]
Switch the debug mode of the engine on and off. In debug mode the engine should sent additonal infos to the GUI, e.g. with the “info string” command, to help debugging, e.g. the commands it the engine has reveived etc. This mode should be switched off per default and this command can be sent any time, also when the engine is thinking.


setoption name <id> [value <x> ]
This is sent to the engine when the user wants to change the internal paramters of the engine. For the “button” type no value is needed. One string will be sent for each paramter and this will only be sent when the engine is waiting. The name of the option in should not be case sensitive!
Here are some strings for the example below:

“setoption name Nullmove value true\n”
“setoption name Selectivity value 3\n”
“setoption name Style value Risky\n”
“setoption name Clear Hash\n”
“setoption name NalimovPath value c :\chess\tb\4;c:\chess\tb\5\n”
register
This is the command to try to register an engine or to tell the engine that registration will be done later. This command should always be sent if the engine has send “registration error” at program startup.
The following tokens are allowed:

later
the user doesn’t want to register the engine now.

name <x>
the engine should be registered with the name <x>

code <y>
the engine should be registered with the code <y>

Example:
“register later”
“register name Stefan MK code 4359874324”
ucinewgame
This is sent to the engine when the next search (started with “position” and “go”) will be from a different game. This can be a new game the engine should play or a new game it should analyse but also the next position from a testsuite with positions only. If the GUI hasn’t sent a “ucinewgame” before the first “position” command, the engine shouldn’t expect any further ucinewgame commands as the GUI is probably not supporting the ucinewgame command. So the engine should not rely on this command even though all new GUIs should support it. As the engine’s reaction to “ucinewgame” can take some time the GUI should always send “isready” after “ucinewgame” to wait for the engine to finish its operation.

position [fen <fenstring> | startpos ] moves <move1>…. <movei>
Set up the position described in fenstring on the internal board and play the moves on the internal chess board. If the game was played from the start postion the string “startpos” will be sent.
Note: no “new” command is needed.

go
Start calculating on the current position. There are a number of commands that can follow this command, all will be sent in the same string. If one command is not send its value should be interpreted as it would not influence the search.

searchmoves <move1> ….<movei>
restrict search to this moves only

ponder
start searching in pondering move. Do not exit the search in ponder mode, even if it’s mate! This means that the last move sent in in the position string is the ponder move.The engine can do what it wants to do, but after a “ponderhit” command it should execute the suggested move to ponder on.

wtime <x>
white has x msec left on the clock

btime <x>
black has x msec left on the clock

winc <x>
white increment per move in mseconds if x > 0

binc <x>
black increment per move in mseconds if x > 0

movestogo <x>
here are x moves to the next time control, this will only be sent if x > 0, if you don’t get this and get the wtime and btime it’s sudden death

depth <x>
search x plies only

nodes <x>
search x nodes only

mate <x>
search for a mate in x moves

movetime <x>
search exactly x mseconds

infinite
search until the “stop” command. Do not exit the search without being told so in this mode!

stop
stop calculating as soon as possible, don’t forget the “bestmove” command when finishing the search!

ponderhit
user has played the expected move. This will be sent if the engine is pondering on the same move the user has played. The engine should continue searching but switch from pondering to normal search.

quit
quit the program as soon as possible
*/