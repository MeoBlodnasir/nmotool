/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sections.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/19 18:04:28 by aduban            #+#    #+#             */
/*   Updated: 2017/01/19 18:25:20 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

char	get_corresponding_sect(uint8_t nsect, t_sect *sects)
{
	t_sect *tmp;

	tmp = sects;
	while (tmp)
	{
		if (tmp->i == nsect)
		{
			if (!ft_strcmp(tmp->name, SECT_DATA))
				return ('D');
			else if (!ft_strcmp(tmp->name, SECT_BSS))
				return ('B');
			else if (!ft_strcmp(tmp->name, SECT_TEXT))
				return ('T');
			else
				return ('S');
		}
		tmp = tmp->next;
	}
	return ('?');
}

char	get_elem_type(struct nlist_64 *array, int i, int t)
{
	if (t == N_PBUD)
		return ('U');
	else if (t == N_UNDF)
	{
		if (array[i].n_value != 0)
			return ('C');
		else
			return ('U');
	}
	else if (t == N_ABS)
		return ('A');
	else if (t == N_INDR)
		return ('I');
	else
		return ('?');
}

t_elem	*add_elem(t_elem *elems, t_elem *elem)
{
	t_elem *tmp;

	if (elems == NULL)
	{
		elems = elem;
		elems->next = NULL;
		elems->prev = NULL;
	}
	else
	{
		tmp = elems;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = elem;
		tmp->next->prev = tmp;
	}
	return (elems);
}

void	switch_elems(t_elem **tmp)
{
	t_elem *tmp2;

	tmp2 = malloc(sizeof(t_elem));
	tmp2->value = (*tmp)->value;
	tmp2->type = (*tmp)->type;
	tmp2->str = (*tmp)->str;
	(*tmp)->value = (*tmp)->next->value;
	(*tmp)->type = (*tmp)->next->type;
	(*tmp)->str = (*tmp)->next->str;
	(*tmp)->next->value = tmp2->value;
	(*tmp)->next->type = tmp2->type;
	(*tmp)->next->str = tmp2->str;
}

t_elem	*sort_elems(t_elem *elems)
{
	t_elem *tmp;
	t_elem *tmp2;
	t_elem *other;

	tmp = elems;
	tmp2 = malloc(sizeof(t_elem));
	other = elems;
	while (other)
	{
		tmp = elems;
		while (tmp)
		{
			if (tmp->next && ft_strcmp(tmp->str, tmp->next->str) > 0)
				switch_elems(&tmp);
			tmp = tmp->next;
		}
		other = other->next;
	}
	free(tmp2);
	return (elems);
}
