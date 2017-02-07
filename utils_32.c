/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/19 18:21:25 by aduban            #+#    #+#             */
/*   Updated: 2017/02/07 17:12:29 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

t_elem		*fill_elem_32(struct nlist *array, int i, t_sect *sects,
		char *stringtable)
{
	t_elem	*elem;
	int		t;

	elem = malloc(sizeof(t_elem));
	t = (uint8_t)(array[i].n_type) & N_TYPE;
	if (((uint8_t)(array[i].n_sect)) != NO_SECT && t == N_SECT)
		elem->type =
			get_corresponding_sect(((uint8_t)(array[i].n_sect)), sects);
	else
		elem->type = get_elem_type_32(array, i, t);
	elem->str = stringtable + swap_32(array[i].n_un.n_strx);
	if ((array[i].n_type & N_EXT) == 0 && elem->type != '?')
		elem->type = ft_tolower(elem->type);
	if ((t & N_STAB) != 0 || elem->type == '?' ||
			!ft_strcmp(elem->str, "") || elem->type == 'u')
	{
		free(elem);
		return (NULL);
	}
	return (elem);
}

void		print_list_32(t_elem *elems)
{
	t_elem	*tmp;

	tmp = elems;
	while (tmp)
	{
		if (tmp->type != 'U')
			ft_printf("%08x %c %s\n", tmp->value, tmp->type, tmp->str);
		else
			ft_printf("         %c %s\n", tmp->type, tmp->str);
		tmp = tmp->next;
	}
}

void		fill_list_32(struct symtab_command *sym, char *ptr, t_sect *sects)
{
	int				i;
	char			*stringtable;
	struct nlist	*array;
	t_elem			*elem;
	t_elem			*elems;

	elems = NULL;
	array = (void*)ptr + swap_32(sym->symoff);
	stringtable = (void*)ptr + swap_32(sym->stroff);
	i = -1;
	while (++i < (int)swap_32(sym->nsyms))
	{
		elem = fill_elem_32(array, i, sects, stringtable);
		if (!elem)
			continue ;
		elem->value = swap_32(array[i].n_value);
		elem->next = NULL;
		elems = add_elem(elems, elem);
	}
	elems = sort_elems(elems);
	print_list_32(elems);
}

void		handle_segv(void *ptr, int size, void *totest)
{
	static void	*start;
	static void	*end;

	if (ptr)
	{
		start = ptr;
		end = start + size;
		return ;
	}
	if (totest < ptr || totest >= end)
	{
		ft_printf("error in file\n");
		exit(0);
	}
}
