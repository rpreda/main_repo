#ifndef SHELL_H
# define SHELL_H
# include "libft.h"
# include "get_next_line.h"
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <stdio.h>
# include <fcntl.h>
# include <dirent.h>
typedef struct	s_lst
{
	char	*data;
	char	rdirect;
	struct s_lst	*next;
}				t_lst;
#endif
