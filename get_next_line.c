#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int is_has_newline(char *buffer)
{
    if (!buffer)
        return (0);

    while (*buffer)
    {
        if (*buffer++ == '\n')
        return (1);
    }
    return (0);
}

size_t  ft_strlen(char *str)
{
    size_t  length;

    if (!str)
        return (0);

    length = 0;
    while (str[length])
        length++;

    return (length);
}

char    *ft_append_text(char *saved_text, char *buffer)
{
    char    *new_text;
    size_t  s_length;
    size_t  b_length;
    size_t  i;
    size_t  j;

    s_length = ft_strlen(saved_text);
    b_length = ft_strlen(buffer);

    new_text = (char *) malloc(s_length + b_length + 1);
    if (!new_text)
        return (NULL);
    
    i = 0;
    while (i < s_length)
    {
        new_text[i] = saved_text[i];
        i++;
    }

    j = 0;
    while (j < b_length)
        new_text[i++] = buffer[j++];
   
    new_text[i] = '\0';
    free(saved_text);

    return (new_text);
}

char    *ft_extract_line(char *saved_text)
{
    size_t  i;
    char    *line;

    if (!saved_text || saved_text[0] == '\0')
        return (NULL);
    i = 0;
    while (saved_text[i] && saved_text[i] != '\n')
        i++;
    if (saved_text[i] == '\n')
        line = (char *) malloc(sizeof(char) * (i + 2)); // \n \0
    else
        line = (char *) malloc(sizeof(char) * (i + 1)); // \0
    if (!line)
        return (NULL);
    
    i = 0;
    while (saved_text[i] && saved_text[i] != '\n')
    {
        line[i] = saved_text[i];
        i++;
    }
    if (saved_text[i] == '\n')
    {
        line[i] = saved_text[i];
        i++;
    }
    line[i] = '\0';
    return (line);
    // loop in saved_text (buffer) whit 0 to mew line or NULL
}

char    *ft_update_saved_text(char *saved_text)
{
    int i;
    int j;
    char *new_saved;

    i = 0;
    while (saved_text[i] && saved_text[i] != '\n')
        i++;    
    
    if (!saved_text[i])
    {
        free(saved_text);
        return (NULL);
    }

    new_saved = (char *) malloc(sizeof(char) * (ft_strlen(saved_text) - i));
    if (!new_saved)
    {
        free(saved_text);
        return (NULL);
    }

    i++;
    j = 0;
    while (saved_text[i])
        new_saved[j++] = saved_text[i++];
    new_saved[j] = '\0';
    free(saved_text);
    return (new_saved);
}

char    *get_next_line(int fd)
{
    static char *saved_text;
    char        *tmp;
    char        *buffer;
    ssize_t     bytes_read;
    char        *line;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    
    buffer = (char *) malloc(sizeof(char) * (BUFFER_SIZE + 1));

    if (!buffer)
        return (NULL);

    bytes_read = 1;

    while (!is_has_newline(saved_text) && bytes_read != 0)
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read == -1)
        {
            free(buffer);
            return (NULL);
        }
        buffer[bytes_read] = '\0';
        tmp = ft_append_text(saved_text, buffer);
        if (!tmp)
        {
            free(saved_text);
            saved_text = NULL;
            free(buffer);
            return (NULL);
        }

        saved_text = tmp;
    }

    free(buffer);

    if (!saved_text || *saved_text == '\0')
    {
        free(saved_text);
        saved_text = NULL;
        return (NULL);
    }

    line = ft_extract_line(saved_text);
    if (!line)
        return (NULL);
    saved_text = ft_update_saved_text(saved_text);

    return (line);
}

int main (void)
{
    int     fd;
    char    *line;

    fd = open("file.txt", O_RDONLY);
    if (fd < 0)
        return (1);

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    
    close(fd);
    return (0);
}