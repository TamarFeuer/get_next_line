/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfeuer <tfeuer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:16:31 by tfeuer            #+#    #+#             */
/*   Updated: 2024/01/03 17:34:26 by tfeuer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_list	*head;
	char			*result;

	if (!initialize_list(fd, &head))
		return (NULL);
	if (head->bytes_unmalloced)
	{
		if (is_full_line_in_buf(&head, &result))
			return (result);
		if (!add_new_node(head))
			return (free_list(&head), NULL);
	}
	if (!read_until_eol(&head, fd))
		return (NULL);
	result = malloc_and_move_head_to_tail(&head);
	if (!result)
		return (NULL);
	head->bytes_unmalloced -= (head->eol + 1);
	if (head->bytes_unmalloced == 0)
		free_list(&head);
	return (result);
}

int	read_until_eol(t_list **head, int fd)
{
	t_list	*cur;

	if ((*head)->next)
		cur = (*head)->next;
	else
		cur = *head;
	while (cur)
	{
		cur->bytes_unmalloced = read(fd, cur->buf, BUFFER_SIZE);
		if (cur->bytes_unmalloced == -1 || (*head)->bytes_unmalloced == 0)
			return (free_list(head), 0);
		if (cur->bytes_unmalloced != BUFFER_SIZE)
			cur->is_eof = 1;
		cur->buf[cur->bytes_unmalloced] = '\0';
		cur->eol = find_endofline(cur);
		if (cur->eol == -1 && !cur->is_eof)
			if (!add_new_node(cur))
				return (free_list(head), 0);
		cur = cur->next;
	}
	return (1);
}

char	*malloc_and_move_head_to_tail(t_list **head)
{
	t_list	*cur;
	char	*line;
	size_t	i;
	ssize_t	j;

	line = (char *) malloc(line_size(*head) + 1);
	if (!line)
		return (free_list(head), NULL);
	i = 0;
	while ((*head)->next)
	{
		cur = *head;
		(*head) = (*head)->next;
		j = cur->eol + 1;
		while (cur->buf[j])
			line[i++] = cur->buf[j++];
		free(cur);
	}
	j = 0;
	while (j <= (*head)->eol)
		line[i++] = (*head)->buf[j++];
	line[i] = '\0';
	return (line);
}

int	is_full_line_in_buf(t_list **head, char **result)
{
	ssize_t	i;
	ssize_t	eonl;
	ssize_t	next_line_size;

	eonl = find_endofline(*head);
	if (eonl != -1)
	{
		next_line_size = eonl - (*head)->eol;
		*result = (char *) malloc(next_line_size + 1);
		if (!*result)
			return (free_list(head), 1);
		i = 0;
		(*head)->eol++;
		while (i < next_line_size)
			(*result)[i++] = (*head)->buf[(*head)->eol++];
		(*result)[i] = '\0';
		(*head)->bytes_unmalloced -= next_line_size;
		if ((*head)->bytes_unmalloced)
			(*head)->eol = eonl;
		else
			free_list(head);
		return (1);
	}
	return (0);
}

// int main()
// {
// 	int fd;
// 	char *lines;
// 	int i;

// 	fd = open("output.txt", O_RDONLY);
// 	lines = get_next_line(fd);
// 	i = 1;
// 	while (lines) 
// 	{
// 		printf("line %d: %s\n", i, lines);
// 		free (lines);
// 		lines = get_next_line(fd);
// 		i++;
// 	}	
// 	close(fd);

// // 	//Read from standard input (fd = 0)
// // 	char *line;
// // 	line = get_next_line(0);
// // 	if (line)
// // 	{
// // 		printf("Read from fd 0: %s\n", line);
// // 		free(line);
// // 	}
// 	return (0);
// }