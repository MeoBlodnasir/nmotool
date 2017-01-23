/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sections.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/19 18:04:28 by aduban            #+#    #+#             */
/*   Updated: 2017/01/23 14:28:47 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

char	get_elem_type_32(struct nlist *array, int i, int t)
{
	if (t == N_PBUD)
		return ('U');
	else if (t == N_UNDF)
	{
		if (swap_32(array[i].n_value) != 0)
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
