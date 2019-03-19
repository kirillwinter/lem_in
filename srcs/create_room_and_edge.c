/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_room_and_edge.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rschuppe <rschuppe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/08 13:34:37 by rschuppe          #+#    #+#             */
/*   Updated: 2019/03/09 18:12:58 by rschuppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

t_room	*create_room(char *name, char *x, char *y, int8_t type)
{
	t_room	*room;

	if (name[0] == 'L' || ft_strstr(name, "-"))
		throw_error(STR_ERROR_VALID, "Forbidden room name");
	SECURE_MALLOC(room = (t_room*)ft_memalloc(sizeof(t_room)));
	SECURE_MALLOC(room->name = ft_strdup(name));
	SECURE_MALLOC(room->paths = ft_dlst_create());
	room->x = min_atoi(x, STR_ERROR_VALID, "Invalid coordinates of the room");
	room->y = min_atoi(y, STR_ERROR_VALID, "Invalid coordinates of the room");
	room->types = type;
	room->weight = -1;
	return (room);
}

int		create_edge(t_farm *farm, char *parent_name, char *child_name)
{
	int		parent;
	int		child;
	t_list	*el;

	parent = find_node_idx_by_name(farm->rooms, farm->count_rooms, parent_name);
	child = find_node_idx_by_name(farm->rooms, farm->count_rooms, child_name);
	if (parent == -1 || child == -1)
		return (0);
	if (parent == child)
		return (1);
	if (parent == 0 || child == 0)
		farm->start_edges++;
	if (parent == farm->count_rooms - 1 || child == farm->count_rooms - 1)
		farm->finish_edges++;
	IS_FLAG(FLAG_DEBUG) && ft_printf("[DEBUG]: Add edge %s[%d] - %s[%d]\n",
		parent_name, parent, child_name, child);
	el = ft_lstnew((void*)&child, sizeof(child));
	ft_lstpush(&farm->rooms[parent]->neighbors, el);
	el = ft_lstnew((void*)&parent, sizeof(child));
	ft_lstpush(&farm->rooms[child]->neighbors, el);
	return (1);
}
