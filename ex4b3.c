/*
* ex4b3.c: Named Pipe
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
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
// ------------------------- Define ----------------------------
#define MAX_TEXT 50
#define MAX_ARR 6
// ------------------------ Prototype --------------------------
char get_command();
void print_answer(FILE *file_read_apps);
// -------------------------- Main -----------------------------
int main()
{
	FILE *file_read_server, *file_write_server;
	FILE *file_read_apps, *file_write_apps;
	char command;
	char string[MAX_TEXT];
	int num;

	file_read_server = fopen("write_fifo_server", "w");
	file_write_server = fopen("read_fifo_server", "r");

	file_read_apps = fopen("write_fifo_apps", "w");
	file_write_apps = fopen("read_fifo_apps", "r");


	if (!file_read_server || !file_write_server
		|| !file_read_apps || !file_write_apps)
	{
		perror("cannot open my_fifo pipe for read/write");
		exit(EXIT_FAILURE);
	}

	fprintf(file_read_server, "%d\n", (int)getpid());
	fflush(file_read_server);
	fscanf(file_write_server, "%d", &num);


	if (num == 0)
	{
		while (1)
		{
			fprintf(file_read_apps, "%d", getpid());
			fflush(file_read_apps);
			command = get_command();
			if (command == 'n')
			{
				fprintf(file_read_apps, "%c\n", command);
				fflush(file_read_apps);

				puts("Enter number to check if prime number \n");
				scanf("%d", &num);
				fprintf(file_read_apps, "%d\n", num);
				fflush(file_read_apps);
				print_answer(file_write_apps);
			}
			else if (command == 's')
			{
				fprintf(file_read_apps, "%c\n", command);
				fflush(file_read_apps);
				puts("Enter string to check if palindrome string \n");
				scanf("%s", string);
				fprintf(file_read_apps, " %s\n", string);
				fflush(file_read_apps);
				print_answer(file_write_apps);
			}
			else
				break;
		}
	}
	else
		puts("Unable to join the server");
	return EXIT_SUCCESS;
}
//----------------------- get_command --------------------------
char get_command()
{
	char command;

	puts("Enter command: 'n', 's' or 'e'\n");
	scanf("%c", &command);
	return command;
}
//----------------------- print_answer --------------------------
void print_answer(FILE *file_read_apps)
{
	int answer;
	fscanf(file_read_apps, "%d", &answer);
	printf("%d\n", answer);
}