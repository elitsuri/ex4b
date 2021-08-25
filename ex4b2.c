/*
* ex4b2.c: Named Pipe
*
*  Created on: Dec 5, 2017
*      Author: eliyhots
*          Id: 201610672
*/
// --------------------- Include Section -----------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
// ---------------------- Define ------------------------------
#define MAX_TEXT 50
// ---------------------- Prototype ---------------------------
void do_command(FILE *file_read, FILE *file_write);
bool check_prime(FILE *file_read);
bool check_palindrome(FILE *file_read);
void signal_handler(int signal);
// ------------------------- Main -----------------------------
int main()
{
	signal(SIGTTIN, signal_handler);
	FILE *file_read_apps;
	FILE *file_write_apps;
	long temp;


	if (mkfifo("read_fifo_apps", S_IFIFO | 0644) == -1)
	{
		perror("cannot create read_fifo file");
		kill(getpid(), SIGTTIN);
	}
	if (mkfifo("write_fifo_apps", S_IFIFO | 0644) == -1)
	{
		perror("cannot create write_fifo file");
		kill(getpid(), SIGTTIN);
	}

	file_read_apps = fopen("read_fifo_apps", "r");
	file_write_apps = fopen("write_fifo_apps", "w");
	if (!file_read_apps || !file_write_apps)
	{
		perror("cannot open my_fifo pipe for read/write");
		kill(getpid(), SIGTTIN);
	}

	while (1)
	{
		fscanf(file_read_apps, "%ld", &temp);
		fprintf(file_write_apps, "write_fifo_apps");
		fflush(file_write_apps);

		fscanf(file_read_apps, "%ld", &temp);

		if (temp == 1)
			do_command(file_read_apps, file_write_apps);
		else
			break;
	}
	return EXIT_SUCCESS;
}
//----------------------- do_command --------------------------
void do_command(FILE *file_read_apps, FILE *file_write_apps)
{
	char command;
	fscanf(file_read_apps, "%c", &command);

	if (command == 'n')
	{
		if (check_prime(file_read_apps))
			fprintf(file_write_apps, "%d", 0);
		else
			fprintf(file_write_apps, "%d", 1);
		fflush(file_write_apps);
	}
	if (command == 's')
	{
		if (check_palindrome(file_read_apps))
			fprintf(file_write_apps, "%d", 0);
		else
			fprintf(file_write_apps, "%d", 1);
		fflush(file_write_apps);
	}
}
//---------------------- check_prime --------------------------
bool check_prime(FILE *file_read)
{
	int num;
	int div;

	fscanf(file_read, "%d", &num);

	for (div = 2; div <= num / 2; div++)
		if (num % div == 0)
			return false;
	return true;
}
//-------------------- check_palindrome -----------------------
bool check_palindrome(FILE *file_read)
{
	char string[MAX_TEXT];

	while (fgets(string, MAX_TEXT, file_read) != NULL)
		;

	int start = 0;
	int end = strlen(string);

	while (start < end)
	{
		if (string[start] != string[end])
			return false;
		start++;
		end--;
	}
	return true;
}
// -------------------- signal_handler -----------------------
void signal_handler(int signal)
{
	exit(EXIT_FAILURE);
}
