#include "lemin.h"
void free_matrix(char **matrix);
char *g_start;
char *g_end;
int	g_room;
int matrix_len(char **matrix)
{
	int i;

	i = 0;
	if (matrix)
	{
		while (matrix[i])
			i++;
	}
	return (i);
}

void matrix_push(char ***matrix, char *elem)
{
	int		i;
	char	**new;
	int		len;

	i = 0;
	len = matrix_len(*matrix);
	new = (char **)malloc(sizeof(char *) * (len + 2));
	new[len + 1] = 0;
	if (*matrix)
	{
		while ((*matrix)[i])
		{
			new[i] = ft_strdup((*matrix)[i]);
			i++;
		}
		free_matrix(*matrix);
	}
	new[len] = elem;
	(*matrix) = new;
}

void free_matrix(char **matrix)
{
	int i;

	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

void lst_add(t_ndlist **nodes, char *name)
{
	t_ndlist *new;
	t_ndlist *aux;

	aux = *nodes;
	new = (t_ndlist *)malloc(sizeof(t_ndlist));
	new->node_name = name;
	new->visited = 0;
	new->edges = 0;
	new->next = 0;
	new->prev = 0;
	if (*nodes == 0)
		*nodes = new;
	else
	{
		while (aux->next)
			aux = aux->next;
		aux->next = new;
	}
}

void read_input(t_ndlist **list, char ***links)//parse
{
	char *line;
	char **mat;
	int special;

	special = 0;
	while (get_next_line(0, &line))
	{
		if (ft_strcmp(line, "") == 0 || ft_strlen(line) == 0)
			break;
		mat = ft_strsplit(line, ' ');
		if (line[0] != '#')
		{
			if (matrix_len(mat) == 3 && is_number(mat[1]) && is_number(mat[2]))
			{
				lst_add(list, ft_strdup(mat[0]));
				if (special == 1)
				{
					g_start = ft_strdup(mat[0]);
					special = 0;
				}
				if (special == 2)
				{
					g_end = ft_strdup(mat[0]);
					special = 0;
				}
			}
			else
			{
				if (count_ch(line, '-') == 1)
					matrix_push(links, ft_strdup(line));
				else
				{
					ft_putendl_fd(ft_strjoin("Wrong line: ", line),2);
					exit(2);
				}
			}
		}
		if (ft_strcmp(line, "##start") == 0)
			special = 1;
		if (ft_strcmp(line, "##end") == 0)
			special = 2;
		free_matrix(mat);
		free(line);
	}
}

int main()
{
	t_ndlist *graph;
	t_ndlist *result;
	char **links;

	graph = 0;
	read_input(&graph, &links);
	if (!graph || !g_start || !g_end)
	{
		ft_putendl_fd("Incorrect input data", 2);
		exit(2);
	}
	process_input(graph, links);
	bfs(get_node(graph, g_start), g_end);
	result = get_node(graph, g_end);
	if (ft_strcmp(g_start, g_end) && !(result->prev))
	{
		ft_putendl_fd("No path found", 2);
		exit(1);
	}
	while (result)
	{
		printf("%s\n", result->node_name);
		result = result->prev;
	}
	/*while (graph)
	{
		printf("%s\n",graph->node_name);
		ln = graph->edges;
		while (ln)
		{
			printf("\t%s\n", ln->link->node_name);
			ln = ln->next;
		}
		graph = graph->next;
	}
	printf("\nSTART %s, END %s\n", g_start, g_end);*/
	return (0);
}
