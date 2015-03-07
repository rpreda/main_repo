#include "queue.h"

t_que *g_queue;

void q_push(t_ndlist *node)
{
	t_que *new;

	new = (t_que *)malloc(sizeof(t_que));
	new->next = 0;
	new->node = node;
	if (!g_queue)
		g_queue = new;
	else
	{
		new->next = g_queue;
		g_queue = new;
	}
}

t_ndlist *q_pop()
{
	t_que *temp;
	t_ndlist *ret_val;
	t_que *free_helper;

	temp = g_queue;
	if (!g_queue)
		return (0);
	if (!g_queue->next)
	{
		ret_val = g_queue->node;
		free(g_queue);
		g_queue = 0;
		return (ret_val);
	}
	while (temp->next->next)
		temp = temp->next;
	ret_val = temp->next->node;
	free_helper = temp->next;
	temp->next = 0;
	free(free_helper);
	return (ret_val);
}
