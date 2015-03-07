#include "queue.h"
int main()
{
	q_push((t_ndlist *)1);
	q_push((t_ndlist *)2);
	q_push((t_ndlist *)3);
	q_push((t_ndlist *)4);
	test();
	printf("Popped elem 1 %p\n", q_pop());
	printf("Popped elem 2 %p\n", q_pop());
	test();
	printf("Popped elem 3 %p\n", q_pop());
	printf("Popped elem 4 %p\n", q_pop());
	test();
	printf("Popped NULL %p\n", q_pop());
	printf("Popped NULL %p\n", q_pop());
	printf("Popped NULL %p\n", q_pop());
	printf("Pushing 1 and 2\n");
	q_push((t_ndlist *)1);
	q_push((t_ndlist *)2);
	test();
	printf("Popped elem 1 %p\n", q_pop());
	printf("Popped elem 2 %p\n", q_pop());
	printf("Popped NULL %p\n", q_pop());
	test();
	return (0);
}
