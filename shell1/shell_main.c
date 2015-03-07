#include "sh1.h"
//PUT errors on error output with ft_putstr_fd ;)

char *current_path;
char **env_data;
int matrix_len(char **matrix);
char *grab_path(char *name, char **env);

char **matrix_dup(char **matrix)
{
	int i;
	char **ret_val;

	i = 0;
	ret_val = ft_memalloc(sizeof(char *) * (matrix_len(matrix) + 1));
	while (matrix[i])
	{
		ret_val[i] = ft_strdup(matrix[i]);
		i++;
	}
	return (ret_val);
}
int matrix_len(char **matrix)
{
	int count;

	count = 0;
	while (matrix[count])
		count++;
	return (count);
}
char *str_before_equal(char *str)
{
	int i;
	char *ret_val;

	i = 0;
	ret_val = NULL;
	while (str[i] && str[i] != '=')
		i++;
	ret_val = ft_memalloc(sizeof(char) * (i + 1));
	ft_strncpy(ret_val, str, i);
	return (ret_val);
}
int	lookup_env(char *keyword)
{
	int i;
	char *free_helper;
	i = 0;
	while (env_data[i])
	{
		free_helper = str_before_equal(env_data[i]);
		if (ft_strcmp(free_helper, keyword) == 0)
			return (i);
		free(free_helper);
		i++;
	}
	return (-1);
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
	matrix = NULL;
}
void put_prompt()
{
	ft_putstr("\x1B[32m$> \x1B[0m");
}
int char_count(char *str, char c)
{
	int count;

	count = 0;
	while(*str)
	{
		if (*str == c)
			count++;
		str++;
	}
	return (count);
}
char *replacement_help_2(char *start, char *keyword, char *ending, char *str)
{
	int i;
	char **temp;
	char *ret_val;
	char *free_helper;

	i = lookup_env(keyword);
	if (i != -1)
	{
		temp = ft_strsplit(env_data[i], '=');
		keyword = ft_strdup(temp[1]);
		free_matrix(temp);
	}
	else
		return (ft_strdup(str));
	ret_val = ft_strjoin(start, keyword);
	free(start);
	free(keyword);
	free_helper = ret_val;
	ret_val = ft_strjoin(ret_val, ending);
	free(free_helper);
	free(ending);
	return (ret_val);
}
char *replacement_helper(int start, char *str)
{
	int part_len;
	char *begining;
	char *ending;
	char *part;
	int i;

	begining = ft_memalloc(sizeof(char) * (start + 1));
	ft_strncpy(begining, str, start);
	i = start;
	while (str[i] && !ft_isspace(str[i]))
		i++;
	part_len = i - start;
	ending = ft_memalloc(ft_strlen(str + i) + 2);
	ft_strcpy(ending, str + i);
	part = ft_memalloc(part_len + 1);
	ft_strncpy(part, str + start + 1, part_len - 1);
	return (replacement_help_2(begining, part, ending, str));
}
void env_replacement(char **data)
{
	int i;
	int j;
	char *free_helper;

	i = 0;
	while (data[i])
	{
		j = 0;
		while (data[i][j])
		{
			if (data[i][j] == '$')
			{
				free_helper = data[i];
				data[i] = replacement_helper(j, data[i]);
				free(free_helper);
			}
			j++;
		}
		i++;
	}
}
void cd_builtin(char **data)
{
	if (chdir(data[1]) == 0)
		current_path = getcwd(NULL, 0);
	else
		ft_putstr("Invalid directory \n");
}
void unsetenv_builtin(char **data)
{
	int poz;
	char *free_helper;

	poz = lookup_env(data[1]);
	if (poz != -1)
	{
		free_helper = env_data[poz];
		while (env_data[poz])
		{
			env_data[poz] = env_data[poz + 1];
			poz++;
		}
		free(free_helper);
	}
	else
		ft_putstr("Env variable doesn't exist, set it using setenv\n");
}
void setenv_builtin(char **data)
{
	char **new_env;
	char *free_helper;
	int i;

	i = 0;
	if (matrix_len(data) != 3)
		ft_putstr("Bad usage of setenv command\n");
	else
	{
		new_env = ft_memalloc(sizeof(char *) * (matrix_len(env_data) + 2));
		while (env_data[i])
		{
			new_env[i] = ft_strdup(env_data[i]);
			i++;
		}
		free_matrix(env_data);
		new_env[i] = ft_strjoin(data[1], "=");
		free_helper = new_env[i];
		new_env[i] = ft_strjoin(new_env[i], data[2]);
		free(free_helper);
		env_data = new_env;
	}
}
int is_builtin(char **data)
{
	if (!ft_strcmp("cd", data[0]))
	{
		cd_builtin(data);
		return (1);
	}
	if (!ft_strcmp("exit", data[0]))
		exit(0);
	if (!ft_strcmp("unsetenv", data[0]))
	{
		unsetenv_builtin(data);
		return (1);
	}
	if (!ft_strcmp("setenv", data[0]))
	{
		setenv_builtin(data);
		return (1);
	}
	printf("Entered for env but got out\n");
	return (0);
}
char *grab_path(char *name, char **env)
{
	char **bin_dirs;
	char **temp;
	char *tmp;
	char *free_help;
	int i;

	i = lookup_env("PATH");
	if (i == -1)
		return (NULL);
	temp = ft_strsplit(env[i], '=');
	bin_dirs = ft_strsplit(temp[1], ':');
	free_matrix(temp);
	i = 0;
	while (bin_dirs[i])
	{
		free_help = ft_strjoin("/", name);
		tmp = ft_strjoin(bin_dirs[i], free_help);
		free(free_help);
		if (access(tmp, R_OK) == 0)
			break ;
		free(tmp);
		tmp = NULL;
		i++;
	}
	free_matrix(bin_dirs);
	return (tmp);
}
void exec_bin(char **data, char **env)
{
	char *exec_path;
	pid_t pid;

	exec_path = grab_path(data[0], env);
	if (exec_path)
	{
		pid = fork();
		if (pid == 0)
		{
			execve(exec_path, data, env_data);
		}
		else
			wait(NULL);
	}
	else
		ft_putstr("Command not found\n");
}
void exec_path(char **data)
{
	pid_t pid;

	if (access(data[0], R_OK) == 0)
	{
		pid = fork();
		if (pid == 0)
		{
			if (execve(data[0], data, env_data) == -1)
				ft_putstr("File can not be executed\n");
			exit(1);
		}
		else
			wait(NULL);
	}
	else
		ft_putstr("File not found\n");
}
void got_command(char **env, char *line)
{
	char **data;
	int		i;
	int		aux;

	if (!ft_strchr(line, 34) && *line != '.' && *line != '/')
	{
		data = ft_strsplit(line, ' ');
		env_replacement(data);
		if (!is_builtin(data))
		{
			if (!strchr(data[0], '/'))
				exec_bin(data, env);
			else
				exec_path(data);
		}
		free_matrix(data);
		printf("Env if 1\n");
	}
	else
	{
		printf("Env if 2\n");
		if (char_count(line, 34) % 2 == 0)
		{
			aux = 0;
			i = 0;
			while (line[i])
			{
				if (!aux)
					if (line[i] == ' ')
						line[i] = 5;
				if (line[i] == 34 && aux == 0)
				{
					line[i] = 5;
					aux = 1;
				}
				if (line[i] == 34 && aux == 1)
				{
					line[i] = 5;
					aux = 0;
				}
				i++;
			}
			data = ft_strsplit(line, 5);
			env_replacement(data);
			if (!is_builtin(data))
			{
				if (!strchr(data[0], '/'))
					exec_bin(data, env);
				else
					exec_path(data);
			}
			free_matrix(data);
		}
		else
			ft_putstr("Please enter an even '\"' number\n");
	}
}
void catch_signal(int signal_catch)
{
	if (signal_catch == SIGINT || signal_catch == SIGQUIT)
	{
		ft_putstr("\n");
		put_prompt();
		signal(SIGINT, catch_signal);
		signal(SIGQUIT, catch_signal);
	}
}
int main(int argc, char **argv, char **env)
{
	char *line;
	char prompt;

	signal(SIGINT, catch_signal);
	signal(SIGQUIT, catch_signal);
	prompt = 0;
	(void)argc;
	(void)argv;
	env_data = matrix_dup(env);
	current_path = getcwd(NULL, 0);
	while (1)
	{
		if (!prompt)
		{
			put_prompt();
			prompt++;
		}
		if (get_next_line(0, &line) > 0)
		{
			prompt = 0;
			if (ft_strlen(line) > 0)
				got_command(env_data, line);
		}
		free(line);
	}
}
