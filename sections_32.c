/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sections.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/19 18:07:53 by aduban            #+#    #+#             */
/*   Updated: 2017/01/20 18:21:33 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

t_sect	*add_section_32(struct segment_command *lc, t_sect *sects, int mark)
{
	struct section	*sec;
	static int			n = 1;
	int					i;

	if (mark)
	{
		n = 1;
		return (NULL);
	}
	sec = (struct section*)(lc + sizeof(lc) / sizeof(void*));
	i = 0;
	while (i < (int)swap_32(lc->nsects))
	{
		sects = add_sec_tolist(n, (char*)sec->sectname, sects);
		n++;
		sec++;
		i++;
	}
	return (sects);
}

t_sect	*get_sections_32(char *ptr, int ncmds, struct segment_command *lc)
{
	t_sect	*sects;
	int		i;

	(void)ptr;
	sects = NULL;
	i = -1;
	while (++i < ncmds)
	{
		if (swap_32(lc->cmd) == LC_SEGMENT)
			sects = add_section_32(lc, sects, 0);
		lc = (void*)lc + swap_32(lc->cmdsize);
	}
	add_section_32(lc, sects, 1);
	return (sects);
}
