/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_sections.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/19 18:32:07 by aduban            #+#    #+#             */
/*   Updated: 2017/01/23 16:37:26 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

void	add_section_32(struct segment_command *lc, int mark, char *ptr)
{
	struct section	*sec;
	int				j;

	(void)mark;
	sec = (struct section*)(lc + sizeof(lc) / sizeof(void*));
	j = -1;
	while (++j < (int)swap_32(lc->nsects))
	{
		if (!ft_strncmp(sec->sectname, SECT_TEXT, sizeof(SECT_TEXT)))
		{
			ft_printf("Contents of (__TEXT,__text) section\n");
			print_otool_32(sec, ptr);
		}
		sec++;
	}
}

t_sect	*get_sections_32(char *ptr, int ncmds, struct segment_command *lc)
{
	t_sect	*sects;
	int		i;

	sects = NULL;
	i = -1;
	while (++i < ncmds)
	{
		if (swap_32(lc->cmd) == LC_SEGMENT)
			add_section_32(lc, 0, ptr);
		lc = (void*)lc + swap_32(lc->cmdsize);
	}
	return (sects);
}

void	handle_32(char *ptr)
{
	t_norm					norm;
	struct mach_header		*header;
	struct load_command		*lc;
	t_sect					*sects;

	header = (struct mach_header *)ptr;
	norm.ncmds = header->ncmds;
	lc = (void *)ptr + sizeof(*header);
	norm.i = 0;
	sects = get_sections_32(ptr, norm.ncmds, (struct segment_command*)lc);
	return ;
}
