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

char	*ft_extract_line(char *saved_text)
{
	size_t	i;
	size_t	line_length;
	ssize_t	newline_index;
	char	*line;

	line_length = ft_strlen(saved_text);
	newline_index = is_has_newline(saved_text);
	if (newline_index >= 0)
		line_length = newline_index + 1;
	line = (char *) malloc(sizeof(char) * (line_length + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (i < line_length)
	{
		line[i] = saved_text[i];
		i++;
	}
	line[i] = '\0';
	return (line);
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
	static t_data	data;
	char			*line;

	if (fd < 0 || fd >= FD_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	if (!data.saved_text)
		data.capacity = 0;
	data.saved_text = ft_read(fd, data.saved_text, &data.capacity);
	if (!data.saved_text || !*data.saved_text)
		return (ft_free_all(NULL, &data.saved_text, NULL, &data.capacity));
	line = ft_extract_line(data.saved_text);
	if (!line)
		return (NULL);
	if (!ft_update_saved_text(&data.saved_text))
		return (ft_free_all(NULL, &data.saved_text, line, &data.capacity));
	if (data.saved_text)
		data.capacity = ft_strlen(data.saved_text) + 1;
	else
		data.capacity = 0;
	return (line);
}
