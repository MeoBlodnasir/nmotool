/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/19 18:21:25 by aduban            #+#    #+#             */
/*   Updated: 2017/01/20 18:30:14 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void		print_list(t_elem *elems)
{
	t_elem	*tmp;

	tmp = elems;
	while (tmp)
	{
		if (tmp->type != 'U')
			ft_printf("%016x %c %s\n", tmp->value, tmp->type, tmp->str);
		else
			ft_printf("                 %c %s\n", tmp->type, tmp->str);
		tmp = tmp->next;
	}
}

t_elem		*fill_elem(struct nlist_64 *array, int i, t_sect *sects,
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
		elem->type = get_elem_type(array, i, t);
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

void		fill_list(struct symtab_command *sym, char *ptr, t_sect *sects)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*array;
	t_elem			*elem;
	t_elem			*elems;

	elems = NULL;
	array = (void*)ptr + swap_32(sym->symoff);
	stringtable = (void*)ptr + swap_32(sym->stroff);
	i = -1;
	while (++i < (int)swap_32(sym->nsyms))
	{
		elem = fill_elem(array, i, sects, stringtable);
		if (!elem)
			continue ;
		elem->value = swap_64(array[i].n_value);
		elem->next = NULL;
		elems = add_elem(elems, elem);
	}
	elems = sort_elems(elems);
	print_list(elems);
}

int			get_name_size(char *name)
{
	int size;

	size = ft_atoi(ft_strchr(name, '/') + 1);
	return (size);
}

uint32_t	swap(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}


int64_t		swap_endian_64(int64_t i)
{
	return ((i >> 0x38) & 0x00000000000000ffLL)
		| ((i >> 0x28) & 0x000000000000ff00LL)
		| ((i >> 0x18) & 0x0000000000ff0000LL)
		| ((i >> 0x08) & 0x00000000ff000000LL)
		| ((i << 0x08) & 0x000000ff00000000LL)
		| ((i << 0x18) & 0x0000ff0000000000LL)
		| ((i << 0x28) & 0x00ff000000000000LL)
		| ((i << 0x38) & 0xff00000000000000LL);
}

int	set_swap(int i)
{
	static int s = 0;
	if (i == 0)
		s = 0;
	else if (i == 1)
		s = 1;
	return (s);
}
uint32_t		swap_32(uint32_t i)
{
	int g;
	g = set_swap(-1);
	if (g == 1)
		return (swap(i));
	return (i);
}

int64_t		swap_64(int64_t i)
{
	int g;
	g = set_swap(-1);
	if (g == 1)
		return (swap_endian_64(i));
	return (i);
}


int	set_swap_fat(int i)
{
	static int s = 0;
	if (i == 0)
		s = 0;
	else if (i == 1)
		s = 1;
	return (s);
}
uint32_t		swap_32_fat(uint32_t i)
{
	int g;
	g = set_swap_fat(-1);
	if (g == 1)
		return (swap(i));
	return (i);
}

int64_t		swap_64_fat(int64_t i)
{
	int g;
	g = set_swap_fat(-1);
	if (g == 1)
		return (swap_endian_64(i));
	return (i);
}
