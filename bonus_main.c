#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line_bonus.h"

char    *get_next_line(int fd);


int main(void)
{
    int     fd1;
    int     fd2;
    // char    *line;
    int     line_count;

    fd1 = open("test1.txt", O_RDONLY);
    fd2 = open("test2.txt", O_RDONLY);

    if (fd1 == -1 && fd2 == -1)
    {
        printf("Error: Could not open test.txt\n");
        return (1);
    }

    line_count = 1;

    // while ((line = get_next_line(fd1)) != NULL)
    // {
    //     printf("Line %d: %s", line_count, line);
    //     free(line);
    //     line_count++;
    // }

    printf("Line %d: %s", line_count, get_next_line(fd1));
    line_count++;

    printf("Line %d: %s", line_count, get_next_line(fd2));
    line_count++;

    printf("Line %d: %s", line_count, get_next_line(fd1));
    line_count++;

    printf("Line %d: %s", line_count, get_next_line(fd2));
    line_count++;


    printf("Line %d: %s", line_count, get_next_line(fd1));
    line_count++;

    printf("Line %d: %s", line_count, get_next_line(fd2));
    line_count++;


    close(fd1);
    close(fd2);
    return (0);
}
