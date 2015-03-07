#include "lemin.h"

int list_len(t_ndlist *list)
{
	int i;

	i = 0;
	while (list)
	{
		i++;
		list = list->next;
	}
	return (i);
}

int get_nodeid(t_ndlist *nodes, char *name)
{
	int i;

	i = 0;
	while (nodes && ft_strcmp(name, nodes->node_name))
	{
		i++;
		nodes = nodes->next;
	}
	return (i);
}

void clean_graph(int len, char **graph)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (i < len)
	{
		j = 0;
		while (j < len)
		{
			graph[i][j] = (char)-1;
			j++;
		}
		i++;
	}
}

void build_graph(char **links, char **graph, t_ndlist *lst)
{
	char **temp;
	int i;

	i = 0;
	while (links[i])
	{
		temp = ft_strsplit(links[i], '-');
		graph[get_nodeid(lst, temp[0])][get_nodeid(lst, temp[1])] = 1;
		graph[get_nodeid(lst, temp[1])][get_nodeid(lst, temp[0])] = 1;
		printf("%d\n", get_nodeid(lst, "node3"));
		free_matrix(temp);
		i++;
	}
	free_matrix(links);
}

char **process_input(t_ndlist *nodes, char **links)
{
	int len;
	char **ret_val;
	int i;

	len = list_len(nodes);
	ret_val = (char **)malloc(sizeof(char *) * (len + 1));
	printf("len %d\n", len);
	ret_val[len] = 0;
	i = 0;
	while (i < len)
	{
		printf("Allocating shit\n");
		ret_val[i] = (char *)malloc(sizeof(char) * (len + 1));
		ret_val[i][len] = 0;
		i++;
	}
	ret_val[0][0] = (char)1;
	printf("Crap is here\n");
	clean_graph(len, ret_val);
	build_graph(links, ret_val, nodes);
	return (ret_val);
}
