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

char	*ft_mem_alloc(char *saved_text, size_t *capacity, size_t required)
{
	char	*new_text;
	size_t	new_capacity;

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
	return (saved_text);
}

char	*ft_strjoin(char *saved_text, char *buffer, size_t *capacity)
{
	size_t	required;
	size_t	saved_length;
	size_t	buffer_length;
	size_t	i;

	saved_length = ft_strlen(saved_text);
	buffer_length = ft_strlen(buffer);
	if (saved_length == SIZE_MAX || buffer_length > SIZE_MAX - saved_length - 1)
		return (NULL);
	required = saved_length + buffer_length + 1;
	saved_text = ft_mem_alloc(saved_text, capacity, required);
	if (!saved_text)
		return (NULL);
	i = 0;
	while (i < buffer_length)
	{
		saved_text[saved_length + i] = buffer[i];
		i++;
	}
	saved_text[saved_length + i] = '\0';
	return (saved_text);
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
