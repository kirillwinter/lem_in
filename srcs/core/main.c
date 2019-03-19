/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rschuppe <rschuppe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 14:17:13 by rschuppe          #+#    #+#             */
/*   Updated: 2019/03/09 19:55:34 by rschuppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static void		clear_paths_comb(t_path_comb *paths_combs)
{
	int		i;

	if (paths_combs)
	{
		i = 0;
		while (paths_combs[i].count)
		{
			free(paths_combs[i].paths);
			i++;
		}
		free(paths_combs);
	}
}

static void		clear_farm(t_farm *farm)
{
	size_t	i;

	if (farm)
	{
		i = 0;
		while (i < farm->intersections_size)
			free(farm->intersections[i++]);
		free(farm->intersections);
		i = 0;
		while ((int)i < farm->count_rooms)
		{
			free(farm->rooms[i]->name);
			ft_lstdel(&farm->rooms[i]->neighbors, free_lst);
			ft_dlst_del(&farm->rooms[i]->paths, free_paths);
			free(farm->rooms[i]);
			i++;
		}
		free(farm->rooms);
		free(farm);
	}
}

static void		start_algo(t_farm *farm)
{
	t_path_comb	*paths_combs;

	if (!(farm->rooms[0]->types == (ROOM_START | ROOM_END)))
	{
		START_DELAY;
		paths_combs = find_unique_paths(farm,
			MIN(farm->start_edges, farm->finish_edges));
		END_DELAY("Find pathes delay");
		IS_FLAG(FLAG_DEBUG) && debug_show_paths_combs(paths_combs);
		START_DELAY;
		!IS_FLAG(FLAG_DEBUG) &&
			let_ants_to_paths(farm, choose_best_comb_paths(paths_combs));
		END_DELAY("Let ants delay");
		START_DELAY;
		clear_paths_comb(paths_combs);
		END_DELAY("Clear memory delay");
	}
	else
		show_map_config(farm);
}

static t_farm	*create_farm(void)
{
	size_t	i;
	t_farm	*farm;

	SECURE_MALLOC(farm = (t_farm*)ft_memalloc(sizeof(t_farm)));
	SET_MAX(farm->max_paths);
	farm->intersections_size = 100;
	SECURE_MALLOC(farm->intersections =
		(bool**)ft_memalloc(farm->intersections_size * sizeof(bool*)));
	i = -1;
	while (++i < farm->intersections_size)
		SECURE_MALLOC(farm->intersections[i] =
			(bool*)ft_memalloc(farm->intersections_size * sizeof(bool)));
	return (farm);
}

int				main(int argc, char **argv)
{
	int			fd;
	char		*filename;
	t_farm		*farm;

	farm = create_farm();
	filename = read_args(argc, argv, &farm->flags, &farm->max_paths);
	START_DELAY;
	(fd = filename ? open(filename, O_RDONLY) : 0) == -1
		&& throw_error(STR_ERROR_SYS, strerror(errno));
	read_farm_map(fd, farm);
	END_DELAY("Read farm delay");
	IS_FLAG(FLAG_DEBUG) && debug_show_rooms(farm);
	start_algo(farm);
	clear_farm(farm);
	return (EXIT_SUCCESS);
}
