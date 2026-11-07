/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaboualk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 10:44:14 by yaboualk          #+#    #+#             */
/*   Updated: 2026/11/06 11:05:19 by yaboualk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdint.h>
# include <unistd.h>
# include <stdlib.h>

// Defined `BUFFER_SIZE`, and set default value to 42,
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

// Defined `FD_MAX` and set default value to `1024`.
# define FD_MAX 1024

// Prototype of functions that exist on `get_next_line.c`
ssize_t	is_has_newline(char *saved_text);
char	*ft_extract_line(char *saved_text);
char	*ft_read(int fd, char *saved_text, size_t *capacity);
char	*ft_free_all(char *buffer, char **saved_text,
			char *line, size_t *capacity);
char	*get_next_line(int fd);

// Prototype of functions that exist on `get_next_line_utils.c`
size_t	ft_strlen(char *str);
char	*ft_mem_alloc(char *saved_text, size_t *capacity, size_t required);
char	*ft_strjoin(char *saved_text, char *buffer, size_t *capacity);
char	*ft_subdup(char *s, size_t start);
int		ft_update_saved_text(char **saved_text);

#endif
