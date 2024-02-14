/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfeuer <tfeuer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:28:00 by tfeuer            #+#    #+#             */
/*   Updated: 2024/01/04 16:43:24 by tfeuer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	add_new_node(t_list *cur)
{
	cur->next = (t_list *) malloc(sizeof(t_list));
	if (!cur->next)
		return (0);
	cur->next->bytes_unmalloced = 0;
	cur->next->eol = -1;
	cur->next->is_eof = 0;
	cur->next->next = NULL;
	return (1);
}

size_t	line_size(t_list *cur)
{
	size_t	len;

	len = 0;
	while (cur->next)
	{
		len += cur->bytes_unmalloced;
		cur = cur->next;
	}
	len += cur->eol + 1;
	return (len);
}

ssize_t	find_endofline(t_list *cur)
{
	ssize_t	i;

	i = cur->eol + 1;
	while (cur->buf[i])
	{
		if (cur->buf[i] == '\n')
			return (i);
		i++;
	}
	if (cur->is_eof)
		return (i - 1);
	else
		return (-1);
}

void	free_list(t_list **head)
{
	t_list	*cur;

	while (*head)
	{
		cur = *head;
		*head = (*head)->next;
		free(cur);
	}
	return ;
}

int	initialize_list(int fd, t_list **head)
{
	if (fd < 0 || BUFFER_SIZE < 1)
		return (0);
	if (!*head)
	{
		*head = (t_list *) malloc(sizeof(t_list));
		if (!*head)
			return (0);
		(*head)->buf[0] = '\0';
		(*head)->bytes_unmalloced = 0;
		(*head)->eol = -1;
		(*head)->is_eof = 0;
		(*head)->next = NULL;
	}
	return (1);
}
