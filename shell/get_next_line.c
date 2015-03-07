/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpreda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/18 15:59:21 by rpreda            #+#    #+#             */
/*   Updated: 2014/11/19 17:17:28 by rpreda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_memconcat(char *s1, char *s2)
{
	char *ret_str;

	ret_str = NULL;
	ret_str = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (s1)
	{
		ft_strcpy(ret_str, s1);
		free(s1);
	}
	if (s2)
		ft_strcat(ret_str, s2);
	if (!s1 && !s2)
		return (NULL);
	else
		return (ret_str);
}

int			ft_status(int val, char *str)
{
	while (str && *str)
	{
		if (*str == '\n')
			return (1);
		str++;
	}
	if (BUFF_SIZE != val)
		return (0);
	return (-2);
}

char		*int_ret_str(char **passed_data, char *s)
{
	char	*line;
	int		len;

	len = 0;
	line = NULL;
	if (s)
	{
		while (s[len] && s[len] != '\n')
			len++;
		line = (char *)malloc(len + 1);
		line[len] = 0;
		ft_strncpy(line, s, len);
		s = s + len + 1;
		*passed_data = ft_strdup(s);
	}
	return (line);
}

int			get_next_line(int const fd, char **line)
{
	int			end_status;
	static char *passed_data[100];
	char		*buf;
	char		*work_string;
	int			val;

	if (fd < 0)
		return (-1);
	val = BUFF_SIZE;
	buf = (char *)malloc(BUFF_SIZE + 1);
	work_string = ft_memconcat(passed_data[fd], NULL);
	end_status = ft_status(val, work_string);
	end_status = -2;
	while (end_status == -2)
	{
		ft_bzero((void *)buf, BUFF_SIZE + 1);
		val = read(fd, buf, BUFF_SIZE);
		work_string = ft_memconcat(work_string, buf);
		end_status = ft_status(val, work_string);
	}
	*line = int_ret_str(&passed_data[fd], work_string);
	free(work_string);
	return (end_status);
}
