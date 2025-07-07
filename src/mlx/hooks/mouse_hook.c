#include <RTmlx.h>
#include <render.h>
#include <mathRT.h>

//	Static Function
static void	mouse_clicks_on_obj(t_scene *sc, t_ray ray);

void	scroll_fov_hook(double xdelta, double ydelta, t_scene *sc)
{
	(void)(xdelta);
	if (ydelta > 0.0 && sc->camera.c.fov > 0.1)
	{
		sc->camera.c.fov = clamp(sc->camera.c.fov - sc->camera.c.cam_fov_speed, 0.0, 180.0);
		sc->camera.c.zvp_dist = 1.0 / tanf((sc->camera.c.fov * M_PI / 180.0) / 2.0);
		sc->render = true;
	}
	else if (ydelta < 0.0 && sc->camera.c.fov < FOV_MAX)
	{
		sc->camera.c.fov = clamp(sc->camera.c.fov + sc->camera.c.cam_fov_speed, 0.0, 180.0);
		sc->camera.c.zvp_dist = 1.0 / tanf((sc->camera.c.fov * M_PI / 180.0) / 2.0);
		sc->render = true;
	}
}

void	mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, t_rt *rt)
{
	int32_t xy[2];
	t_vec4	ndc;
	t_ray	ray;
	float	x;
	float	y;

	(void)mods;
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS)
	{
		mlx_get_mouse_pos(rt->win->mlx, &xy[X], &xy[Y]);
		x = ((float)xy[X] / (float)rt->win->res_ratio) + 0.5F;
		y = ((float)xy[Y] / (float)rt->win->res_ratio) + 0.5F;
		ndc[X] = (2.0F * (x / (float)rt->win->rndr_wdth) - 1.0F) * rt->win->aspectrat;
		ndc[Y] = 1.0F - 2.0F * (y / (float)rt->win->rndr_hght);
		ndc[Z] = rt->scene->camera.c.zvp_dist;
		ray.origin = rt->scene->camera.coords;
		ray.vec = transform_ray_dir(ndc, rt->scene->camera.c.orientation);
		mouse_clicks_on_obj(rt->scene, ray);
	}
}


static void	mouse_clicks_on_obj(t_scene *sc, t_ray ray)
{
	float		closest_t;
	uint8_t		closest_intersect_type;
	t_objs		*closest_obj;
	uint32_t	closest_obj_index;

	closest_obj_index = find_closest_object(sc, ray, &closest_t, &closest_intersect_type);
	closest_obj = sc->objs + closest_obj_index;
	
	closest_obj = render_light(sc, ray, &closest_t, closest_obj);
	if (closest_t < INFINITY && closest_t > 0.0F)
	{
		sc->render = true;
		if (sc->selected_obj == NULL || closest_obj != sc->selected_obj)
		{
			if (closest_obj->type != LIGHT)
				sc->sel_obj_index = closest_obj_index;
			sc->selected_obj = closest_obj;
		}
		else
		{
			sc->selected_obj = NULL;
		}
	}
}

