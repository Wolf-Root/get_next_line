#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

// includes
#include <unistd.h>
#include <stdlib.h>

// define
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1
#endif


// Function Prototype
// void	get_next_line(int fd);

char *get_next_line(int fd);

#endif
