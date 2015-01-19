#include "shell.h"

void catch_signal(int signal_catch)
{
	if (signal_catch == SIGQUIT)
		exit(0);
	if (signal_catch == SIGINT || signal_catch == SIGQUIT)
	{
		ft_putstr("\n");
		put_prompt();
		signal(SIGINT, catch_signal);
		signal(SIGQUIT, catch_signal);
	}
}

void put_prompt()
{
	ft_putstr("\x1B[32m$> \x1B[0m");
}

void ft_parse(char *line)
{
	char **data;
	t_lst *list;
	char *quote;
	int		i;

	quote = 1;
	i = 0;
	while (*(line + i))
	{
		if (i != 0 && (line[i] == '"' && line[i - 1] != '\\'))
			quote *= -1;
		else
			if (i == 0 && *line = '"')
				quote *= -1;


		i++;
	}
}

int main(int argc, char **argv, char **envp)
{
	char *line;
	char prompt;
	signal(SIGINT, &catch_signal);
	signal(SIGQUIT, &catch_signal);
	while (1)
	{
		f (!prompt)
		{
			put_prompt();
			prompt++;
		}
		if (get_next_line(0, &line) > 0)
		{
			prompt = 0;
			if (ft_strlen(line) > 0)
				//got_command(env_data, line); Now i have to do shit
		}
		free(line);
	}
	return (0);
}
