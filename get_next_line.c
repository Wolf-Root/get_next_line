/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaboualk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 10:43:20 by yaboualk          #+#    #+#             */
/*   Updated: 2026/11/01 10:16:28 by yaboualk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

ssize_t	is_has_newline(char *saved_text)
{
	ssize_t	i;

	if (!saved_text)
		return (-1);
	i = 0;
	while (saved_text[i])
	{
		if (saved_text[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

char	*ft_strjoin(char *saved_text, char *buffer, size_t *capacity)
{
	char	*new_text;
	size_t	new_capacity;
	size_t	required;
	size_t	saved_length;
	size_t	buffer_length;
	size_t	i;

	saved_length = ft_strlen(saved_text);
	buffer_length = ft_strlen(buffer);
	if (saved_length == SIZE_MAX || buffer_length > SIZE_MAX - saved_length - 1)
		return (NULL);
	required = saved_length + buffer_length + 1;
	if (required > *capacity)
	{
		new_capacity = *capacity;
		if (new_capacity == 0)
			new_capacity = 16;
		while (new_capacity < required)
		{
			if (new_capacity > SIZE_MAX / 2)
			{
				new_capacity = required;
				break ;
			}
			new_capacity *= 2;
		}
		new_text = (char *) realloc(saved_text, new_capacity);
		if (!new_text)
			return (NULL);
		saved_text = new_text;
		*capacity = new_capacity;
	}
	i = 0;
	while (i < buffer_length)
	{
		saved_text[saved_length + i] = buffer[i];
		i++;
	}
	saved_text[saved_length + i] = '\0';
	return (saved_text);
}

char	*ft_free_all(char *buffer, char **saved_text,
			char *line, size_t *capacity)
{
	free(buffer);
	free(*saved_text);
	free(line);
	*saved_text = NULL;
	*capacity = 0;
	return (NULL);
}

char	*ft_read(int fd, char *saved_text, size_t *capacity)
{
	char		*tmp;
	char		*buffer;
	ssize_t		bytes_read;

	buffer = (char *) malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (ft_free_all(NULL, &saved_text, NULL, capacity));
	bytes_read = 1;
	while (bytes_read != 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (ft_free_all(buffer, &saved_text, NULL, capacity));
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		tmp = ft_strjoin(saved_text, buffer, capacity);
		if (!tmp)
			return (ft_free_all(buffer, &saved_text, NULL, capacity));
		saved_text = tmp;
		if (is_has_newline(buffer) >= 0)
			break ;
	}
	free(buffer);
	return (saved_text);
}

char	*get_next_line(int fd)
{
	static char		*saved_text;
	static size_t	capacity;
	char			*line;

	if (fd < 0 || fd >= FD_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	if (!saved_text)
		capacity = 0;
	saved_text = ft_read(fd, saved_text, &capacity);
	if (!saved_text || !*saved_text)
		return (ft_free_all(NULL, &saved_text, NULL, &capacity));
	line = ft_extract_line(saved_text);
	if (!line)
		return (NULL);
	if (!ft_update_saved_text(&saved_text))
		return (ft_free_all(NULL, &saved_text, line, &capacity));
	if (saved_text)
		capacity = ft_strlen(saved_text) + 1;
	else
		capacity = 0;
	return (line);
}
