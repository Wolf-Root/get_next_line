/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaboualk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 10:44:14 by yaboualk          #+#    #+#             */
/*   Updated: 2026/11/08 21:26:24 by yaboualk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdint.h>
# include <unistd.h>
# include <stdlib.h>

// Defined `BUFFER_SIZE`, and set default value to 42,
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

// Defined `FD_MAX` and set default value to `1024`.
# define FD_MAX 1024

typedef struct s_data
{
	char	*saved_text;
	size_t	capacity;
}	t_data;

// Prototype of functions that exist on `get_next_line_bonus.c`
ssize_t	is_has_newline(char *saved_text);
char	*ft_extract_line(char *saved_text);
char	*ft_free_all(char *buffer, char **saved_text,
			char *line, size_t *capacity);
char	*ft_read(int fd, char *saved_text, size_t *capacity);
char	*get_next_line_BONUS(int fd);

// Prototype of functions that exist on `get_next_line_utils_bonus.c`
size_t	ft_strlen(char *str);
char	*ft_mem_alloc(char *saved_text, size_t *capacity, size_t required);
char	*ft_strjoin(char *saved_text, char *buffer, size_t *capacity);
char	*ft_strdup(char *s, size_t start);
int		ft_update_saved_text(char **saved_text);

#endif
