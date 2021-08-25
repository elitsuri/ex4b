/*
* ex4b1.c: Named Pipe
*
*  Created on: Nov 29, 2017
*      Author: eliyho tsuri
*       Login: eliyhots
*		  Id: 201610672
*
*/
// ------------------- Include Section -----------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <signal.h>
// ----------------------- Define ----------------------------
#define MAX_TEXT 50
#define MAX_ARR 5
// ------------------------ Prototype --------------------------
bool is_on_arr(pid_t arr_pid[], pid_t new_pid);
void signal_handler(int signal);
// ------------------------ Main -----------------------------
int main()
{
	signal(SIGTTIN, signal_handler);
	FILE *file_read_server;
	FILE *file_write_server;
	FILE *file_read_apps;
	FILE *file_write_apps;

	pid_t arr_pid[MAX_ARR];
	long temp;
	int index = 0;

	if (mkfifo("read_fifo_server", S_IFIFO | 0644) == -1)
	{
		perror("cannot create read_fifo file");
		kill(getpid(), SIGTTIN);
	}
	if (mkfifo("write_fifo_server", S_IFIFO | 0644) == -1)
	{
		perror("cannot create write_fifo file");
		kill(getpid(), SIGTTIN);
	}
	file_read_server = fopen("read_fifo_server", "r");
	file_write_server = fopen("write_fifo_server", "w");

	if (!file_read_server || !file_write_server)
	{
		perror("cannot open my_fifo pipe for read/write");
		kill(getpid(), SIGTTIN);
	}
	puts("Both fifo opened");


	while (1)
	{
		fscanf(file_read_server, "%ld", &temp);
		if (index == MAX_ARR)
		{
			fprintf(file_write_server, "%d\n", 2);
			fflush(file_write_server);
		}
		else
		{
			if (!is_on_arr(arr_pid, temp))
			{
				arr_pid[index] = temp;
				index++;
				fprintf(file_write_server, "%d\n", 0);
				fflush(file_write_server);
			}
			else
			{
				fprintf(file_write_server, "%d\n", 1);
				fflush(file_write_server);
			}
		}
		file_read_apps = fopen("write_fifo_apps", "r");
		file_write_apps = fopen("read_fifo_apps", "w");
		fscanf(file_read_apps, "%ld", &temp);
		if (is_on_arr(arr_pid, temp))
			fprintf(file_write_apps, "%d\n", 1);
		else
			fprintf(file_write_apps, "%d\n", 0);
	}
	return EXIT_SUCCESS;
}
// --------------------- is_on_arr ---------------------------
bool is_on_arr(pid_t arr_pid[], pid_t new_pid)
{
	int index;
	bool on_arr = false;

	for (index = 0; index < MAX_ARR; index++)
		if (arr_pid[index] == new_pid)
			on_arr = true;
	return on_arr;
}
// -------------------- signal_handler -----------------------
void signal_handler(int signal)
{
	exit(EXIT_FAILURE);
}