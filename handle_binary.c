/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_binary.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 15:46:09 by aduban            #+#    #+#             */
/*   Updated: 2017/01/23 15:46:48 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	handle_64(char *ptr)
{
	t_norm					norm;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;
	t_sect					*sects;

	header = (struct mach_header_64 *)ptr;
	norm.ncmds = swap_32(header->ncmds);
	lc = (void *)ptr + sizeof(*header);
	norm.i = 0;
	sects = get_sections(ptr, norm.ncmds, (struct segment_command_64*)lc);
	while (++norm.i < norm.ncmds)
	{
		if (swap_32(lc->cmd) == LC_SYMTAB)
		{
			sym = (struct symtab_command*)lc;
			fill_list(sym, ptr, sects);
			return ;
		}
		lc = (void*)lc + swap_32(lc->cmdsize);
	}
}

void	handle_32(char *ptr)
{
	t_norm					norm;
	struct mach_header		*header;
	struct load_command		*lc;
	struct symtab_command	*sym;
	t_sect					*sects;

	header = (struct mach_header *)ptr;
	norm.ncmds = header->ncmds;
	lc = (void *)ptr + sizeof(*header);
	norm.i = 0;
	sects = NULL;
	sects = get_sections_32(ptr, norm.ncmds, (struct segment_command*)lc);
	while (++norm.i < norm.ncmds)
	{
		if (swap_32(lc->cmd) == LC_SYMTAB)
		{
			sym = (struct symtab_command*)lc;
			fill_list_32(sym, ptr, sects);
			return ;
		}
		lc = (void*)lc + swap_32(lc->cmdsize);
	}
}
