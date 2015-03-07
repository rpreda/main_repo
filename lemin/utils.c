#include "lemin.h"

int is_number(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int count_ch(char *s, char c)
{
	int i;
	int k;

	k = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			k++;
		i++;
	}
	return (k);
}
