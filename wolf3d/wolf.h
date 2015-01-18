#ifndef WOLF_H
# define WOLF_H
# include <X.h>
# include <libft.h>
# include "get_next_line.h"
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>
typedef struct	s_point
{
	int			x;
	int			y;
}				t_ipt;
typedef struct	s_point2
{
	double		x;
	double		y;
}				t_dpt;
typedef struct	s_vect
{
	t_dpt		pos;
	t_dpt		dir;
}				t_vect;
typedef struct	s_img
{
	char	*addr;
	int		bitspp;
	int		ln_sz;
	int		endian;
}				t_img;
typedef struct	s_line
{
	t_ipt		start;
	t_ipt		end;
	int			val;
	int			x_map;
	int			y_map;
}				t_line;
typedef struct	s_state
{
	t_dpt		dir;
	t_dpt		plane;
	t_dpt		pos;
}				t_state;
typedef struct	s_lndata
{
	double		camera_x;
	t_vect		ray;
	t_ipt		map_pos;
	t_dpt		side_dist;
	t_dpt		delta_dist;
	double		perp_wall_dist;
	t_ipt		step;
	int			hit;
	int			side;
	int			c;
	int			prev_c;
}				t_ln_data;
typedef struct	s_env
{
	void		*mlx;
	void		*win;
	void		*img;
	int			**map;
	t_state		state;
	int			mov_f;
	int			mov_b;
	int			mov_l;
	int			mov_r;
	int			start;
	char		*sky;
	void		*sky_data;
	int			h;
	int			w;
}				t_env;
#endif
