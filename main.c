#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Prototype for your function
char    *get_next_line(int fd);

int main(void)
{
    int     fd;
    char    *line;
    int     line_count;

    // Open your test file
    fd = open("test.txt", O_RDONLY);
    if (fd == -1)
    {
        printf("Error: Could not open test.txt\n");
        return (1);
    }

    line_count = 1;

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("Line %d: %s", line_count, line);
        free(line);
        line_count++;
    }
    
    close(fd);
    return (0);
}
