/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaboualk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 10:44:30 by yaboualk          #+#    #+#             */
/*   Updated: 2026/10/30 10:44:33 by yaboualk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(char *str)
{
	size_t	length;

	if (!str)
		return (0);
	length = 0;
	while (str[length])
		length++;
	return (length);
}

char	*ft_extract_line(char *saved_text)
{
	size_t	i;
	size_t	line_length;
	ssize_t	newline_index;
	char	*line;

	line_length = ft_strlen(saved_text);
	newline_index = is_has_newline(saved_text);
	if(newline_index >= 0)
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

char	*ft_subdup(char *s, size_t start)
{
	size_t	i;
	char	*dup;

	i = 0;
	while (s[start + i])
		i++;
	if (i == 0)
		return (NULL);
	dup = (char *) malloc(sizeof(char) * (i + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (s[start + i])
	{
		dup[i] = s[start + i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int	ft_update_saved_text(char **saved_text)
{
	size_t	i;
	char	*new_saved;

	i = 0;
	while ((*saved_text)[i] && (*saved_text)[i] != '\n')
		i++;
	if (!(*saved_text)[i] || !(*saved_text)[i + 1])
	{
		free(*saved_text);
		*saved_text = NULL;
		return (1);
	}
	new_saved = ft_subdup(*saved_text, i + 1);
	if (!new_saved)
		return (0);
	free(*saved_text);
	*saved_text = new_saved;
	return (1);
}
