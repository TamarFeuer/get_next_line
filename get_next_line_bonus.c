/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfeuer <tfeuer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:27:50 by tfeuer            #+#    #+#             */
/*   Updated: 2024/01/04 17:51:10 by tfeuer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static t_list	*head[1024];
	char			*result;

	if (!initialize_list(fd, &head[fd]))
		return (NULL);
	if (head[fd]->bytes_unmalloced)
	{
		if (is_full_line_in_buf(&head[fd], &result))
			return (result);
		if (!add_new_node(head[fd]))
			return (free_list(&head[fd]), NULL);
	}
	if (!read_until_eol(&head[fd], fd))
		return (NULL);
	result = malloc_and_move_head_to_tail(&head[fd]);
	if (!result)
		return (NULL);
	head[fd]->bytes_unmalloced -= (head[fd]->eol + 1);
	if (head[fd]->bytes_unmalloced == 0)
		free_list(&head[fd]);
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
// 	int	 fd1, fd2, fd3;
// 	char *line;

// 	fd1 = open("output1.txt", O_RDONLY);
// 	fd2 = open("output2.txt", O_RDONLY);
// 	fd3 = open("output3.txt", O_RDONLY);
// 	line = get_next_line(fd1);
// 	printf("Next line: %s", line);
// 	free (line);
// 	line = get_next_line(fd2);
// 	printf("Next line: %s", line);
// 	free (line);	
// 	line = get_next_line(fd3);
// 	printf("Next line: %s", line);
// 	line = get_next_line(fd1);
// 	printf("Next line: %s", line);
// 	free (line);
// 	line = get_next_line(fd2);
// 	printf("Next line: %s", line);
// 	free (line);	
// 	line = get_next_line(fd3);
// 	printf("Next line: %s", line);
// 	free (line);
// 	close(fd1);
// 	close(fd2);
// 	close(fd3);
// 	return (0);
// }