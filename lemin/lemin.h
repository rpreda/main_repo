#ifndef LEMIN_H
# define LEMIN_H
# include "libft.h"
# include <unistd.h>
# include <stdlib.h>
# include "get_next_line.h"
#include <stdio.h>//REMOVE ME
struct s_link;
typedef struct	s_nodelist
{
	char		*node_name;
	char		visited;
	struct s_link		*edges;
	struct s_nodelist	*next;
	struct s_nodelist	*prev;
}				t_ndlist;
typedef struct	s_link
{
	t_ndlist		*link;
	struct s_link	*next;
}				t_link;
void process_input(t_ndlist *nodes, char **links);
void free_matrix(char **mat);
t_ndlist *get_node(t_ndlist *graph, char *name);
void bfs(t_ndlist *graph, char *end_node);
int is_number(char *str);
int count_ch(char *s, char c);
#endif
