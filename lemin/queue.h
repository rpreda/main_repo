#ifndef QUEUE_H
# define QUEUE_H
# include "lemin.h"
void		q_push(t_ndlist *node);
t_ndlist	*q_pop();
void test();

typedef struct		s_que
{
	t_ndlist *node;
	struct s_que *next;
}					t_que;

#endif
