/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sections.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/19 18:07:53 by aduban            #+#    #+#             */
/*   Updated: 2017/02/07 15:35:07 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

t_sect	*add_sec_tolist(int n, char *name, t_sect *sects)
{
	t_sect	*section;
	t_sect	*tmp;

	section = malloc(sizeof(t_sect));
	section->name = name;
	section->i = n;
	section->next = NULL;
	if (!sects)
		sects = section;
	else
	{
		tmp = sects;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = section;
	}
	return (sects);
}

t_sect	*add_section(struct segment_command_64 *lc, t_sect *sects, int mark)
{
	struct section_64	*sec;
	static int			n = 1;
	int					i;

	handle_segv(NULL, 0, lc);
	if (mark)
	{
		n = 1;
		return (NULL);
	}
	sec = (struct section_64*)(lc + sizeof(lc) / sizeof(void*));
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

t_sect	*get_sections(char *ptr, int ncmds, struct segment_command_64 *lc)
{
	t_sect	*sects;
	int		i;

	handle_segv(NULL, 0, lc);
	(void)ptr;
	sects = NULL;
	i = -1;
	while (++i < ncmds)
	{
		if (swap_32(lc->cmd) == LC_SEGMENT_64)
			sects = add_section(lc, sects, 0);
		lc = (void*)lc + swap_32(lc->cmdsize);
		handle_segv(NULL, 0, lc);
	}
	add_section(lc, sects, 1);
	return (sects);
}
