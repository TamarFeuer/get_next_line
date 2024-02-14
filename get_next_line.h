/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfeuer <tfeuer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:16:40 by tfeuer            #+#    #+#             */
/*   Updated: 2024/01/02 14:16:45 by tfeuer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <limits.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 3
# endif

typedef struct s_list
{
	char			buf[BUFFER_SIZE + 1];
	ssize_t			bytes_unmalloced;
	ssize_t			eol;
	int				is_eof;
	struct s_list	*next;
}	t_list;

char	*get_next_line(int fd);
int		is_full_line_in_buf(t_list **head, char **result);
int		read_until_eol(t_list **head, int fd);
char	*malloc_and_move_head_to_tail(t_list **head);
int		add_new_node(t_list *cur);
size_t	line_size(t_list *cur);
ssize_t	find_endofline(t_list *cur);
void	free_list(t_list **head);
int		initialize_list(int fd, t_list **head);

#endif