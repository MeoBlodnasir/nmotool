/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_sections.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/19 18:32:07 by aduban            #+#    #+#             */
/*   Updated: 2017/01/19 18:48:55 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

void	add_section(struct segment_command_64 *lc, int mark, char *ptr)
{
	struct section_64	*sec;
	int					j;

	(void)mark;
	sec = (struct section_64*)(lc + sizeof(lc) / sizeof(void*));
	j = -1;
	while (++j < (int)lc->nsects)
	{
		if (!ft_strncmp(sec->sectname, SECT_TEXT, sizeof(SECT_TEXT)))
			print_otool(sec, ptr);
		sec++;
	}
}

t_sect	*get_sections(char *ptr, int ncmds, struct segment_command_64 *lc)
{
	t_sect	*sects;
	int		i;

	sects = NULL;
	i = -1;
	while (++i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
			add_section(lc, 0, ptr);
		lc = (void*)lc + lc->cmdsize;
	}
	return (sects);
}

void	handle_64(char *ptr)
{
	t_norm					norm;
	struct mach_header_64	*header;
	struct load_command		*lc;
	t_sect					*sects;

	header = (struct mach_header_64 *)ptr;
	norm.ncmds = header->ncmds;
	lc = (void *)ptr + sizeof(*header);
	norm.i = 0;
	sects = get_sections(ptr, norm.ncmds, (struct segment_command_64*)lc);
	return ;
}
