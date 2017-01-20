/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_archive.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/19 18:23:11 by aduban            #+#    #+#             */
/*   Updated: 2017/01/20 18:51:59 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	handle_fat(char *ptr, int file_size, char *file)
{
	struct fat_header	*fathead;
	struct fat_arch		*arch;
	uint32_t			i;
	uint32_t			offset;

	fathead = (void*)ptr;
	i = swap_32_fat(fathead->nfat_arch);
	arch = (void*)ptr + sizeof(fathead);
	while (i)
	{
		if (swap_32_fat(arch->cputype) == CPU_TYPE_X86_64)
		{
			offset = swap_32_fat(arch->offset);
			nm(ptr + offset, file, file_size, 0);
			return ;
		}
		arch += sizeof(arch) / sizeof(void*);
		i--;
	}
}

void	handle_archive(char *ptr, char *file, uint32_t file_size)
{
	struct ar_hdr	*arch;
	int				size;
	struct ar_hdr	*tmp;

	arch = (void*)ptr + SARMAG;
	size = get_name_size(arch->ar_name);
	arch = (void*)ptr + sizeof(*arch) + SARMAG + size;
	size += *((int*)(arch));
	arch = (void*)ptr + sizeof(*arch) + SARMAG + size + sizeof(int);
	size += *((int*)(arch));
	arch = (void*)ptr + sizeof(*arch) + SARMAG + size +
		sizeof(int) + sizeof(int);
	tmp = arch;
	while (tmp < (struct ar_hdr*)(file_size + (void*)ptr))
	{
	char *str = 		(char*)(ft_strstr(tmp->ar_name, ARFMAG) + ft_strlen(ARFMAG));
	if (ft_strcmp(str, SYMDEF) != 0 && ft_strcmp(str, SYMDEF_SORTED) != 0)
	{
		ft_printf("\n%s(%s):\n", file, (char*)(ft_strstr(tmp->ar_name,
						ARFMAG) + ft_strlen(ARFMAG)));
		nm((char*)(tmp + 1) + get_name_size(tmp->ar_name), NULL, file_size, 0);
	}
		tmp = (void*)tmp + ft_atoi(tmp->ar_size) + sizeof(struct ar_hdr);
	}
}
