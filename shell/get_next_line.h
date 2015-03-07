/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpreda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/17 16:07:35 by rpreda            #+#    #+#             */
/*   Updated: 2014/11/19 17:31:42 by rpreda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GET_NEXT_LINE_H
# define FT_GET_NEXT_LINE_H
# include <unistd.h>
# define BUFF_SIZE 32
# include "libft.h"

int get_next_line(int const fd, char **line);

#endif
