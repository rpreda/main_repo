#include <mlx.h>
#include "wolf.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

int	loop_hook(t_env *env);

void put_pixel(char *image, int x, int y, int ln_sz, int col)
{
	int offset;

	offset = x * 4;
	offset += y * ln_sz;
	*(int *)(image + offset) = col;
}
int		expose_hook(t_env *e)
{
	if (e->img)
		mlx_put_image_to_window(e->mlx, e->win, e->img, 0, 0);
	return (0);
}
void	move_forward(t_env *e)
{
	if (e->map[(int)(e->state.pos.x + e->state.dir.x * 0.2)][(int)(e->state.pos.y)] % 2 == 0)
		e->state.pos.x += e->state.dir.x * 0.2;
	if (e->map[(int)(e->state.pos.x)][(int)(e->state.pos.y + e->state.dir.y * 1)] % 2 == 0)
		e->state.pos.y += e->state.dir.y * 0.2;
}
void	move_back(t_env *e)
{
	if (e->map[(int)(e->state.pos.x - e->state.dir.x * 0.2)][(int)(e->state.pos.y)] % 2 == 0)
		e->state.pos.x -= e->state.dir.x * 0.2;
	if (e->map[(int)(e->state.pos.x)][(int)(e->state.pos.y - e->state.dir.y * 1)] % 2 == 0)
		e->state.pos.y -= e->state.dir.y * 0.2;
}
void	move_left(t_env *e)
{
	double oldPlaneX;
	double oldDirX;

	oldDirX = e->state.dir.x;
	e->state.dir.x = e->state.dir.x * cos(0.1) - e->state.dir.y * sin(0.1);
	e->state.dir.y = oldDirX * sin(0.1) + e->state.dir.y * cos(0.1);
	oldPlaneX = e->state.plane.x;
	e->state.plane.x = e->state.plane.x * cos(0.1) - e->state.plane.y * sin(0.1);
	e->state.plane.y = oldPlaneX * sin(0.1) + e->state.plane.y * cos(0.1);
}
void	move_right(t_env *e)
{
	double oldPlaneX;
	double oldDirX;

	oldDirX = e->state.dir.x;
	e->state.dir.x = e->state.dir.x * cos(-0.1) - e->state.dir.y * sin(-0.1);
	e->state.dir.y = oldDirX * sin(-0.1) + e->state.dir.y * cos(-0.1);
	oldPlaneX = e->state.plane.x;
	e->state.plane.x = e->state.plane.x * cos(-0.1) - e->state.plane.y * sin(-0.1);
	e->state.plane.y = oldPlaneX * sin(-0.1) + e->state.plane.y * cos(-0.1);
}
int		key_hook(int keycode, t_env *e)
{
	if (keycode == 65307)
		exit(0);
	if (keycode == 65362 || keycode == 'w')
		e->mov_f = 1;
	if (keycode == 65364 || keycode == 's')
		e->mov_b = 1;
	if (keycode == 65361 || keycode == 'a')
		e->mov_l = 1;
	if (keycode == 65363 || keycode == 'd')
		e->mov_r = 1;
	return (0);
}
int		release_hook(int keycode, t_env *e)
{
	if (keycode == 65362 || keycode == 'w')
		e->mov_f = 0;
	if (keycode == 65364 || keycode == 's')
		e->mov_b = 0;
	if (keycode == 65361 || keycode == 'a')
		e->mov_l = 0;
	if (keycode == 65363 || keycode == 'd')
		e->mov_r = 0;
	return (0);
}
void	init_graphics(t_env *env)
{
	env->mlx = mlx_init();
	env->win = mlx_new_window(env->mlx, 1280, 768, "Ray-caster");
	mlx_expose_hook(env->win, expose_hook, env);
	mlx_hook(env->win, 2, (1L<<0), &key_hook, env);
	mlx_hook(env->win, 3, (1L<<1), &release_hook, env);
	mlx_loop_hook(env->mlx, loop_hook, env);
	env->sky_data = mlx_xpm_file_to_image(env->mlx, "env.xpm", &env->w, &env->h);
	env->sky = mlx_get_data_addr(env->sky_data, &env->h, &env->w, &env->h);
	mlx_loop(env->mlx);
}
void	init_values(t_env *env)
{
	env->state.pos.x = 28;
	env->state.pos.y = 9;
	env->state.dir.x = -1;
	env->state.dir.y = 0;
	env->state.plane.x = 0;
	env->state.plane.y = 0.66;
	env->mov_f = 0;
	env->mov_b = 0;
	env->mov_l = 0;
	env->mov_r = 0;
	env->start = 0;
}
void	init_line_data(t_env env, int x, t_ln_data *ldata)
{
	ldata->camera_x = 2 * x / (double)(1280) - 1;
	ldata->ray.pos.x = env.state.pos.x;
	ldata->ray.pos.y = env.state.pos.y;
	ldata->ray.dir.x = env.state.dir.x + env.state.plane.x * ldata->camera_x;
	ldata->ray.dir.y = env.state.dir.y + env.state.plane.y * ldata->camera_x;
	ldata->map_pos.x = (int)ldata->ray.pos.x;
	ldata->map_pos.y = (int)ldata->ray.pos.y;
	ldata->delta_dist.x = sqrt(1 + (ldata->ray.dir.y * ldata->ray.dir.y) /
			(ldata->ray.dir.x * ldata->ray.dir.x));
	ldata->delta_dist.y = sqrt(1 + (ldata->ray.dir.x * ldata->ray.dir.x) /
			(ldata->ray.dir.y * ldata->ray.dir.y));
	ldata->hit = 0;
}
void	prepare_dda(t_ln_data *ld)
{
	if (ld->ray.dir.x < 0)
	{
		ld->step.x = -1;
		ld->side_dist.x = (ld->ray.pos.x - ld->map_pos.x) * ld->delta_dist.x;
	}
	else
	{
		ld->step.x = 1;
		ld->side_dist.x = (ld->map_pos.x + 1.0 - ld->ray.pos.x) * ld->delta_dist.x;
	}
	if (ld->ray.dir.y < 0)
	{
		ld->step.y = -1;
		ld->side_dist.y = (ld->ray.pos.y - ld->map_pos.y) * ld->delta_dist.y;
	}
	else
	{
		ld->step.y = 1;
		ld->side_dist.y = (ld->map_pos.y + 1.0 - ld->ray.pos.y) * ld->delta_dist.y;
	}
}
void	dda(t_ln_data *ld, int **world)
{
	while (ld->hit == 0)
	{
		if (ld->side_dist.x < ld->side_dist.y)
		{
			ld->side_dist.x += ld->delta_dist.x;
			ld->map_pos.x += ld->step.x;
			ld->side = 0;
		}
		else
		{
			ld->side_dist.y += ld->delta_dist.y;
			ld->map_pos.y += ld->step.y;
			ld->side = 1;
		}
		if (world[ld->map_pos.x][ld->map_pos.y] > 0)
			ld->hit = 1;
	}
}
t_line	get_line(t_ln_data ln)
{
	t_line	ret_val;
	int		ln_h;

	if (ln.side == 0)
		ln.perp_wall_dist = fabs((ln.map_pos.x - ln.ray.pos.x + (1 - ln.step.x) / 2) / ln.ray.dir.x);
	else
		ln.perp_wall_dist = fabs((ln.map_pos.y - ln.ray.pos.y + (1 - ln.step.y) / 2) / ln.ray.dir.y);
	ln_h = abs((int)(768 / ln.perp_wall_dist));
	ret_val.start.y = -ln_h / 2 + 384;
	if (ret_val.start.y < 0)
		ret_val.start.y = 0;
	ret_val.end.y = ln_h / 2 + 384;
	if (ret_val.end.y >= 768)
		ret_val.end.y = 767;
	return (ret_val);
}
void	tint_color(int *col, int len)
{
	int r;
	int g;
	int b;

	r = *col % 256;
	g = *col / 256 % 256;
	b = *col / 256 / 256 % 256;
	*col = 0;
	if (len > 650)
		len = 650;
	*col += r + ((double)(750 - len) / 1000 * (255 - r));
	*col += 256 * (g + ((double)(750 - len) / 1000 * (255 - g)));
	*col += 256 * 256 * (b + ((double)(750 - len) / 1000 * (255 - b)));
}
void	draw_line(t_line ln, t_img im, int x, int c)
{
	int i;
	int col;
	static int x_cord;
	static int y_cord;

	i = ln.start.y;
	col = c;
	if (ln.val == 2)
		col = 13132848;
	if (x_cord != ln.x_map || y_cord != ln.y_map)
	{
		x_cord = ln.x_map;
		y_cord = ln.y_map;
		col = 0;
	}
	else
	{
		if (ln.val == 3)
		{
			col = 16776960;
			tint_color(&col, ln.end.y - ln.start.y);
		}
	}
	while (i < ln.end.y)
	{
		if ((i < ln.start.y + 1) || (i >= ln.end.y - 1))
			put_pixel(im.addr, x, i, im.ln_sz, 0);
		else
			put_pixel(im.addr, x, i, im.ln_sz, col);
		i++;
	}
}
void	prep_image(t_img im, t_env *env)
{
	int i;
	int j;

	ft_memcpy(im.addr, env->sky, 1966080);
	i = 384;
	j = 0;
	while (i < 768)
	{
		j = 0;
		while (j < 1280)
		{
			put_pixel(im.addr, j, i, im.ln_sz, 9344156);
			j++;
		}
		i++;
	}
}
void	*generate_frame(t_env env)
{
	int x;
	t_ln_data line_data;
	t_line	ln;
	void	*image;
	t_img	im;

	x = 0;
	image = mlx_new_image(env.mlx, 1280, 768);
	im.addr = mlx_get_data_addr(image, &im.bitspp, &im.ln_sz, &im.endian);
	prep_image(im, &env);
	while (x < 1280)
	{
		init_line_data(env, x, &line_data);
		prepare_dda(&line_data);
		dda(&line_data, env.map);
		ln = get_line(line_data);
		ln.val = env.map[line_data.map_pos.x][line_data.map_pos.y];
		ln.x_map = line_data.map_pos.x;
		ln.y_map = line_data.map_pos.y;
		if (line_data.side == 0)
		{
			if (line_data.step.x < 0)
				draw_line(ln, im, x, 255);
			else
				draw_line(ln, im, x, 65280);
		}
		else
			if (line_data.step.y < 0)
				draw_line(ln, im, x, 16711935);
			else
				draw_line(ln, im, x, 16776960);
		x++;
	}
	return (image);
}
int		loop_hook(t_env *env)
{
	if (env->img)
		mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
	if (!env->start || env->mov_f || env->mov_b || env->mov_l || env->mov_r)
	{
		if (env->mov_f)
			move_forward(env);
		if (env->mov_b)
			move_back(env);
		if (env->mov_l)
			move_left(env);
		if (env->mov_r)
			move_right(env);
		env->img = generate_frame(*env);
		mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
		mlx_destroy_image(env->mlx, env->img);
		env->img = NULL;
		env->start = 1;
		env->start = 1;
	}
	return (0);
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
int		*parse_line(char *line)
{
	int		*ret_val;
	char	**temp;
	int		i;

	temp = ft_strsplit(line, ' ');
	ret_val = (int *)malloc(sizeof(int) * (ft_strlen(line) + 2));
	i = 0;
	while (temp[i])
	{
		ret_val[i] = ft_atoi(temp[i]);
		i++;
	}
	free_matrix(temp);
	return (ret_val);
}
int		**read_map(char *filename)
{
	int		**ret_val;
	int		fd;
	char	*buff;
	int		i;

	ret_val = (int **)malloc(sizeof(int *) * 100);
	fd = open(filename, O_RDONLY);
	if (fd < 1)
	{
		perror("wolf3d");
		exit(1);
	}
	i = 0;
	while (get_next_line(fd, &buff))
	{
		ret_val[i] = parse_line(buff);
		free(buff);
		i++;
	}
	return (ret_val);
}
int		main(int argc, char **argv)
{
	t_env env;

	env.img = NULL;
	if (argc > 1)
		env.map = read_map(argv[1]);
	else
	{
		ft_putstr("Please select map! (ex /wolf3d map1.wolf)\n");
		exit(1);
	}
	init_values(&env);
	move_right(&env);
	init_graphics(&env);
	return (0);
}
