#include "lemin.h"

t_ndlist *get_node(t_ndlist *graph, char *name)
{
	while (graph && ft_strcmp(name, graph->node_name))
		graph = graph->next;
	if (!graph)
	{
		ft_putendl_fd(ft_strjoin("Bad chamber name ", name), 2);
		exit(1);
	}
	return (graph);
}

void link_add(t_ndlist *node, t_ndlist *link)
{
	t_link *new;

	if (!node || !link)
		return ;
	new = (t_link *)malloc(sizeof(t_link));
	new->next = 0;
	new->link = link;
	if (!node->edges)
		node->edges = new;
	else
	{
		new->next = node->edges;
		node->edges = new;
	}
}

void process_input(t_ndlist *graph, char **links)
{
	char **temp;
	int i;

	i = 0;
	if (!links)
		return ;
	while (links[i])
	{
		temp = ft_strsplit(links[i], '-');
		link_add(get_node(graph, temp[0]), get_node(graph, temp[1]));
		link_add(get_node(graph, temp[1]), get_node(graph, temp[0]));
		i++;
		free(temp);
	}
}
