/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/19 18:29:32 by aduban            #+#    #+#             */
/*   Updated: 2017/01/20 19:41:18 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

void		print_otool_32(struct section *sec, char *ptr)
{
	void	*start;
	void	*end;
	void	*p;
	int		i;

	start = (void*)ptr + swap_32(sec->offset);
	end = start + swap_32(sec->size);
	p = start;
	i = 0;
	while (p < end)
	{
		ft_printf("%08llx\t", sec->addr + (p - start));
		i = -1;
		while (++i < 16 && p < end)
		{
			ft_printf("%02hhx ", *(char*)p);
			++p;
		}
		ft_printf("\n");
	}
}
