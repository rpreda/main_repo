#include "lemin.h"
#include "queue.h"
void bfs(t_ndlist *graph, char *end_node)
{
	t_ndlist *node;
	t_link *links;

	graph->visited = 1;
	q_push(graph);
	while ((node = q_pop()) != 0)
	{
		links = node->edges;
		node->visited = 1;
		while (links)
		{
			if (!(links->link->visited))
			{
				links->link->prev = node;
				if (!ft_strcmp(end_node, links->link->node_name))
					return ;
				else
				q_push(links->link);
			}
			links = links->next;
		}
	}
}
