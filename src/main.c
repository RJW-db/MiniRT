#include <miniRT.h>
#include <RTerror.h>
// #include <scene.h>
// #include <RTmlx.h>
#include <setup_clean.h>
#include <render.h>

int main(int argc, char **argv)
{
	t_rt		rt;
	t_scene		sc;
	t_scene		read_sc;
	t_window	win;

	init_main(&rt, &sc, &read_sc, &win);
	if (setup_init_parsing(&rt, argc, argv[1]) != EXIT_SUCCESS)
		return (errset(ERTRN));
	init_hooks(&rt);
	if (THREADS > 1)
	{
		rt.scene->render_ongoing = true;
		if (multithreaded(&rt) != EXIT_SUCCESS)
			return (cleanup(&rt), errset(ERTRN));
	}
	mlx_loop(rt.win->mlx);
	cleanup(&rt);
	printf("exiting miniRT\n");
	return (EXIT_SUCCESS);
}
