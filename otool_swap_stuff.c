/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_swap_stuff.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 15:51:01 by aduban            #+#    #+#             */
/*   Updated: 2017/01/23 15:56:29 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

uint32_t		swap_32(uint32_t i)
{
	int g;

	g = set_swap(-1);
	if (g == 1)
		return (swap(i));
	return (i);
}

int64_t			swap_64(int64_t i)
{
	int g;

	g = set_swap(-1);
	if (g == 1)
		return (swap_endian_64(i));
	return (i);
}

int				set_swap_fat(int i)
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

int64_t			swap_64_fat(int64_t i)
{
	int g;

	g = set_swap_fat(-1);
	if (g == 1)
		return (swap_endian_64(i));
	return (i);
}
