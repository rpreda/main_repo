#include "shell.h"

void catch_signal(int signal_catch)
{
	exit(0);//REMOVE ME
	if (signal_catch == SIGINT || signal_catch == SIGQUIT)
	{
		ft_putstr("\n");
		ft_putstr("\x1B[32m$> \x1B[0m");
		signal(SIGINT, catch_signal);
		signal(SIGQUIT, catch_signal);
	}
}
void ft_manage_blocks(char **blocks)
{
	int i;
	int j;
	int found;
	t_lst *lst;

	i = 0;
	while (blocks[i])
	{
		j = 0;
		while (blocks[i][j])
		{
			found = 0;
			if (blocks[i][j] == '<' || blocks[i][j] == '>' || blocks[i][j] == '|')
			{
				if (blocks[i][j] == '<' && blocks[i][j + 1] == '<' && found += 1)
					;//case double in
				else if (blocks[i][j] == '<' && found += 2)
					;//case in
				else if (blocks[i][j] == '>' && blocks[i][j + 1] == '>' && found += 3)
					;//case double out
				else if (blocks[i][j] == '>' && found += 4)
					;//case out
				else if (blocks[i][j] == '|' && found += 6)
					;//case pipe
				if (found)
				{
					
				}
			}
			j++;
		}
		i++;
	}
}
void ft_parse(char *line)
{
	char **data;
	t_lst *list;
	char quote;
	int		i;

	quote = 1;
	i = 0;
	while (line[i])
	{
		if (i != 0 && (line[i] == '"' && line[i - 1] != '\\'))
			quote *= -1;
		else
			if (i == 0 && *line == '"')
				quote *= -1;
		if (quote > 0 && line[i] == ';')
		{
			if (i != 0 && line[i - 1] != '\\')
				line[i] = 5;
			else if (i == 0)
				line[i] = 5;
		}
		i++;
	}
	data = ft_strsplit(line, 5);
	i=0;//
	while (data[i])
	{
		printf("%s\n", data[i]);
		i++;
	}//
	ft_manage_blocks(data);
}

int main(int argc, char **argv, char **envp)
{
	char *line;
	char prompt;
	(void)argc;
	(void)argv;
	signal(SIGINT, &catch_signal);
	signal(SIGQUIT, &catch_signal);
	prompt = 0;
	while (1)
	{
		if (!prompt)
		{
			ft_putstr("\x1B[32m$> \x1B[0m");
			prompt++;
		}
		if (get_next_line(0, &line) > 0)
		{
			prompt = 0;
			if (ft_strlen(line) > 0)
				ft_parse(line);
		}
		free(line);
	}
	return (0);
}
