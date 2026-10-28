#include "get_next_line.h"
#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int	has_newline(char *str)
{
	size_t	i = 0;
	
	while (str[i])
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

void	ft_lines(ssize_t bytes ,char *buffer)
{
	char	line[BUFFER_SIZE + 1];
	static	char next_line[BUFFER_SIZE + 1];
	ssize_t	i;
	ssize_t	j;
		
	i = 0;
	j = has_newline(buffer);

	while (i < j)
	{
		line[i] = buffer[i];
		i++;
	}
		
	line[i] = '\0';
	i = 0;
	while (buffer[j + i])
	{
		next_line[i] = buffer[j + i];
			i++;
	}
	next_line[bytes] = '\0';

	printf("line 	  === %s\n", line);
	printf("next_line === %s\n", next_line);

}

void	get_next_line(int fd)
{
	ssize_t	bytes;
	char 	buffer[BUFFER_SIZE + 1];

	if (fd < 0)
		return ;

	bytes = read(fd, buffer, BUFFER_SIZE);

	if (bytes <= 0)
		return ;
	else
	 	ft_lines(bytes , buffer);

}

int main(void)
{
	int		fd = open("file.txt", O_RDONLY | O_CREAT);

	get_next_line(fd);


	return (0);
}
