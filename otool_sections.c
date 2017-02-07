/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_sections.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/19 18:32:07 by aduban            #+#    #+#             */
/*   Updated: 2017/02/07 15:50:47 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

void	add_section(struct segment_command_64 *lc, int mark, char *ptr)
{
	struct section_64	*sec;
	int					j;

	(void)mark;
	sec = (struct section_64*)(lc + sizeof(lc) / sizeof(void*));
	handle_segv(NULL, 0, sec);
	handle_segv(NULL, 0, lc);
	j = -1;
	(void)ptr;
	while (++j < (int)lc->nsects)
	{
		if (!ft_strncmp(sec->sectname, SECT_TEXT, sizeof(SECT_TEXT)))
		{
			ft_printf("Contents of (__TEXT,__text) section\n");
			print_otool(sec, ptr);
		}
		sec++;
		handle_segv(NULL, 0, sec);
	}
}

t_sect	*get_sections(char *ptr, int ncmds, struct segment_command_64 *lc)
{
	t_sect	*sects;
	int		i;

	sects = NULL;
	i = -1;
	(void)ptr;
	handle_segv(NULL, 0, lc);
	while (++i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			add_section(lc, 0, ptr);
		}
		lc = (void*)lc + lc->cmdsize;
		handle_segv(NULL, 0, lc);
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
