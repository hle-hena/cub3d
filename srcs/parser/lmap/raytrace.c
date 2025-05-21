/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytrace.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:22:39 by hle-hena          #+#    #+#             */
/*   Updated: 2025/05/21 19:14:36 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline t_vec	get_wall_direction(t_vec wall_start, t_vec wall_end, t_vec ray_dir)
{
	float dx = wall_end.x - wall_start.x;
	float dy = wall_end.y - wall_start.y;
	float nx = -dy;
	float ny = dx;
	float dot = nx * ray_dir.x + ny * ray_dir.y;

	if (dot > 0)
	{
		nx = -nx;
		ny = -ny;
	}
	return ((t_vec){nx, ny});
}

int	is_correct_flight(void *content, void *to_find)
{
	t_vec	cont_normal;
	t_vec	find_normal;

	cont_normal = ((t_flight *)content)->normal;
	find_normal = *(t_vec *)to_find;
	return (cont_normal.x == find_normal.x && cont_normal.y == find_normal.y);
}

t_flight	*new_flight(t_vec normal)
{
	t_flight	*flight;

	flight = malloc(sizeof(t_flight));
	if (!flight)
		return (NULL);
	flight->color = 0;
	flight->emittance = 0;
	flight->normal = normal;
	return (flight);
}

static inline t_flight	*hit_light(t_data *data, t_trace *ray, t_wpath wall)
{
	t_vec		normal;
	t_tlight	*tlight;
	t_list		*lst;
	t_flight	*flight;

	normal = get_wall_direction(wall.start, wall.end, ray->dir);
	tlight = (data->lmap.lmap + ray->curr.x + ray->curr.y * data->lmap.wid);
	lst = ft_lstchr(tlight->flight, &normal, is_correct_flight);
	if (!lst)
	{
		flight = new_flight(normal);
		add_link(&tlight->flight, flight);
	}
	else
		flight = (t_flight *)lst->content;
	return (flight);
}

static inline t_tile	*hit_tile(t_data *data, t_trace *ray)
{
	return (get_tile_dict()[*(data->map->matrix + ray->real.y * data->map->wid + ray->real.x)]);
}

static inline int	color_attenuation(t_col col, float emittance)
{
	col.re *= emittance;
	col.gr *= emittance;
	col.bl *= emittance;
	return (calc_color(col));
}

void	calc_trace(t_trace *ray)
{
	ray->step.x = 1;
	if (ray->dir.x < 0)
		ray->step.x = -1;
	ray->step.y = 1;
	if (ray->dir.y < 0)
		ray->step.y = -1;
	ray->slope.x = HUGE_VALF;
	if (ray->dir.x != 0)
		ray->slope.x = fabs(1.0f / ray->dir.x);
	ray->slope.y = HUGE_VALF;
	if (ray->dir.y != 0)
		ray->slope.y = fabs(1.0f / ray->dir.y);
	if (ray->dir.x < 0)
		ray->dist.x = (ray->origin.x - floorf(ray->origin.x)) * ray->slope.x;
	else
		ray->dist.x = (ceilf(ray->origin.x) - ray->origin.x) * ray->slope.x;
	if (ray->dir.y < 0)
		ray->dist.y = (ray->origin.y - floorf(ray->origin.y)) * ray->slope.y;
	else
		ray->dist.y = (ceilf(ray->origin.y) - ray->origin.y) * ray->slope.y;
	ray->curr.x = floorf(ray->origin.x);
	ray->curr.y = floorf(ray->origin.y);
	ray->running = 1;
	ray->precise_dist = 0;
}

static inline float	cross(t_vec a, t_vec b)
{
	return (a.x * b.y - a.y * b.x);
}

static inline float	intersect_segment(t_vec origin, t_vec dir, t_wpath d)
{
	t_vec seg = {d.end.x - d.start.x, d.end.y - d.start.y};

	float det = -dir.x * seg.y + dir.y * seg.x;
	if (fabs(det) < 1e-6f)
		return -1;

	t_vec delta = {d.start.x - origin.x, d.start.y - origin.y};
	float t = (delta.x * -seg.y + delta.y * seg.x) / det;
	float u = (-dir.y * delta.x + dir.x * delta.y) / det;
	if (t >= 0 && u >= 0 && u <= 1)
		return t;
	return -1;
}

static inline t_wpath	line(t_point curr, t_wpath base)
{
	return ((t_wpath){(t_vec){(base.start.x + curr.x) * LMAP_PRECISION, (base.start.y + curr.y) * LMAP_PRECISION},
		(t_vec){(base.end.x + curr.x) * LMAP_PRECISION, (base.end.y + curr.y) * LMAP_PRECISION}, (t_text){0}});
}

static inline float	angle(t_vec ray_dir, t_wpath seg)
{
	t_vec	wall_dir;
	t_vec	wall_normal;
	float	len;
	float	dot_val;

	wall_dir.x = seg.end.x - seg.start.x;
	wall_dir.y = seg.end.y - seg.start.y;
	len = sqrtf(wall_dir.x * wall_dir.x + wall_dir.y * wall_dir.y);
	if (len == 0.0f)
		return (0.0f);
	wall_dir.x /= len;
	wall_dir.y /= len;
	wall_normal.x = -wall_dir.y;
	wall_normal.y = wall_dir.x;
	dot_val = ray_dir.x * wall_normal.x + ray_dir.y * wall_normal.y;
	dot_val = fabs(dot_val);
	if (dot_val < 0)
		dot_val = 0;
	if (dot_val > 1)
		dot_val = 1;
	return (sqrtf(sqrtf(dot_val)));
}


static inline int	does_hit(t_list	*wpath, t_trace *ray, t_wpath *wall,
	float *angle_factor)
{
	float	dist;
	float	temp;

	dist = -1;
	*angle_factor = 0;
	while (wpath)
	{
		temp = intersect_segment(ray->origin, ray->dir,
			line(ray->real, *(t_wpath *)wpath->content));
		if (temp < -0.5f)
		{
			wpath = wpath->next;
			continue ;
		}
		if (temp < dist || dist == -1)
		{
			*wall = *(t_wpath *)wpath->content;
			*angle_factor = angle(ray->dir, *wall);
			dist = temp;
		}
		wpath = wpath->next;
	}
	if (dist < 0)
		return (0);
	ray->precise_dist = dist;
	return (1);
}

void	handle_reflexion(t_data *data, t_trace *ray, t_light light)
{
	t_text		texture;
	t_tile		*tile;
	t_flight	*flight;
	t_vec		hit;
	t_wpath		wall;
	float		before;
	float		emittance;
	float		angle_factor;

	tile = hit_tile(data, ray);
	wall = (t_wpath){0};
	before = ray->precise_dist;
	int expected_x = floorf(ray->origin.x + ray->dir.x * ray->precise_dist + (ray->dir.x > 0 ? 0.001 : -0.001) * !ray->side);
	int expected_y = floorf(ray->origin.y + ray->dir.y * ray->precise_dist + (ray->dir.y > 0 ? 0.001 : -0.001) * ray->side);
	if (!does_hit(tile->wpath, ray, &wall, &angle_factor))
		return ;
	hit.x = ray->origin.x + ray->dir.x * ray->precise_dist;
	hit.y = ray->origin.y + ray->dir.y * ray->precise_dist;
	if ((ray->dir.x > 0 && floorf(hit.x) > expected_x)
		|| (ray->dir.x < 0 && floorf(hit.x) < expected_x)
		|| (ray->dir.y > 0 && floorf(hit.y) > expected_y)
		|| (ray->dir.y < 0 && floorf(hit.y) < expected_y))
		return ((ray->precise_dist = before), VOID);
	texture = wall.texture;
	flight = hit_light(data, ray, wall);
	angle_factor = angle_factor -pow(2.71828, -angle_factor * 0.1 * (ray->precise_dist / LMAP_PRECISION)) + 1;
	if (angle_factor > 1)
		angle_factor = 1;
	emittance = angle_factor * ray->emittance * 1.06f
		- 0.06f * pow(2.71828, 0.5f * (ray->precise_dist / LMAP_PRECISION));
	if (emittance < 0)
		return (ray->running = 0, VOID);
	if (texture.reflectance && ray->bounce < MAX_BOUNCE - 1 && emittance > 0.01)
	{
		ray->origin = hit;
		if (ray->side == 0)
			ray->origin.x += ray->dir.x > 0 ? -0.001f : 0.001f;
		else
			ray->origin.y += ray->dir.y > 0 ? -0.001f : 0.001f;
		if (ray->origin.x < 0.001f)
			ray->origin.x = 0.001f;
		if (ray->origin.y < 0.001f)
			ray->origin.y = 0.001f;
		if (ray->origin.x > data->map->wid - 0.001f)
			ray->origin.x = data->map->wid - 0.001f;
		if (ray->origin.y > data->map->len - 0.001f)
			ray->origin.y = data->map->len - 0.001f;
		if (ray->side == 0)
			ray->dir.x = -ray->dir.x;
		else
			ray->dir.y = -ray->dir.y;
		calc_trace(ray);
		ray->bounce++;
	}
	else
	{
		ray->running = 0;
		flight->color = color_attenuation(light.color, emittance);
		flight->emittance = emittance;
	}
	ray->emittance -= (1 - texture.reflectance);
}

void	init_trace(t_trace *ray, t_vec dir, t_vec origin, float emittance)
{
	origin.x *= LMAP_PRECISION;
	origin.y *= LMAP_PRECISION;
	ray->step.x = 1;
	if (dir.x < 0)
		ray->step.x = -1;
	ray->step.y = 1;
	if (dir.y < 0)
		ray->step.y = -1;
	ray->slope.x = HUGE_VALF;
	if (dir.x != 0)
		ray->slope.x = fabs(1.0f / dir.x);
	ray->slope.y = HUGE_VALF;
	if (dir.y != 0)
		ray->slope.y = fabs(1.0f / dir.y);
	if (dir.x < 0)
		ray->dist.x = (origin.x - floorf(origin.x)) * ray->slope.x;
	else
		ray->dist.x = (ceilf(origin.x) - origin.x) * ray->slope.x;
	if (dir.y < 0)
		ray->dist.y = (origin.y - floorf(origin.y)) * ray->slope.y;
	else
		ray->dist.y = (ceilf(origin.y) - origin.y) * ray->slope.y;
	ray->curr.x = floorf(origin.x);
	ray->curr.y = floorf(origin.y);
	ray->bounce = 0;
	ray->origin = origin;
	ray->dir = dir;
	ray->running = 1;
	ray->precise_dist = 0;
	ray->emittance = emittance;
}

void	handle_attenuation(t_data *data, t_lmap *lmap, t_trace *ray, t_light light)
{
	float	emittance;

	emittance = ray->emittance * 1.06f
		- 0.06f * pow(2.71828, 0.5f * (ray->precise_dist / LMAP_PRECISION));
	if (emittance < 0)
		return (ray->running = 0, VOID);
	if (emittance < (lmap->lmap + ray->curr.x + ray->curr.y * data->map->wid * LMAP_PRECISION)->ce_fl.emittance)
		return ;
	(lmap->lmap + ray->curr.x + ray->curr.y * data->map->wid * LMAP_PRECISION)->ce_fl.color = color_attenuation(light.color, emittance);
	(lmap->lmap + ray->curr.x + ray->curr.y * data->map->wid * LMAP_PRECISION)->ce_fl.emittance = emittance;
}

void	raytrace(t_data *data, t_light light, t_vec dir)
{
	t_trace	ray;

	init_trace(&ray, dir, light.pos, light.emittance);
	handle_attenuation(data, &data->lmap, &ray, light);
	while (ray.running)
	{
		if (ray.dist.x < ray.dist.y)
		{
			ray.precise_dist = ray.dist.x;
			ray.curr.x += ray.step.x;
			ray.dist.x += ray.slope.x;
			ray.side = 0;
		}
		else
		{
			ray.precise_dist = ray.dist.y;
			ray.curr.y += ray.step.y;
			ray.dist.y += ray.slope.y;
			ray.side = 1;
		}
		ray.real.x = ray.curr.x / LMAP_PRECISION;
		ray.real.y = ray.curr.y / LMAP_PRECISION;
		if (ray.curr.x < 0 || ray.curr.x >= data->lmap.wid || ray.curr.y < 0
			|| ray.curr.y >= data->lmap.len)
			return ;
		handle_attenuation(data, &data->lmap, &ray, light);
		if (get_tile_dict()[*(data->map->matrix + ray.real.y * data->map->wid + ray.real.x)]->is_wall)
			handle_reflexion(data, &ray, light);
	}
}
