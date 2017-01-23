/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_fat_archive.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 15:48:49 by aduban            #+#    #+#             */
/*   Updated: 2017/01/23 16:07:31 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

void			handle_fat(t_file f)
{
	struct fat_header	*fathead;
	struct fat_arch		*arch;
	uint32_t			i;
	uint32_t			offset;

	fathead = (void*)f.ptr;
	i = fathead->nfat_arch;
	i = swap(i);
	arch = (void*)f.ptr + sizeof(fathead);
	while (i)
	{
		if (swap(arch->cputype) == CPU_TYPE_X86_64)
		{
			offset = arch->offset;
			f.ptr = f.ptr + swap(offset);
			otool(f, 0);
			return ;
		}
		arch += sizeof(arch) / sizeof(void*);
		i--;
	}
}

struct ar_hdr	*get_sizes(char *ptr)
{
	int				size;
	struct ar_hdr	*arch;

	arch = (void*)ptr + SARMAG;
	size = get_name_size(arch->ar_name);
	arch = (void*)ptr + sizeof(*arch) + SARMAG + size;
	size += *((int*)(arch));
	arch = (void*)ptr + sizeof(*arch) + SARMAG + size + sizeof(int);
	size += *((int*)(arch));
	arch = (void*)ptr + sizeof(*arch) + SARMAG + size +
		sizeof(int) + sizeof(int);
	return (arch);
}

void			handle_archive(char *ptr, char *file, uint32_t file_size)
{
	struct ar_hdr	*arch;
	t_offlist		*lst;
	struct ar_hdr	*tmp;
	t_file			f;
	char			*str;

	lst = NULL;
	arch = get_sizes(ptr);
	tmp = arch;
	while (tmp < (struct ar_hdr*)(file_size + (void*)ptr))
	{
		str = (char*)(ft_strstr(tmp->ar_name, ARFMAG) + ft_strlen(ARFMAG));
		if (ft_strcmp(str, SYMDEF) != 0 && ft_strcmp(str, SYMDEF_SORTED) != 0)
		{
			ft_printf("%s(%s):\n", file, (char*)(ft_strstr(tmp->ar_name,
							ARFMAG) + ft_strlen(ARFMAG)));
			f.ptr = (char*)(tmp + 1) + get_name_size(tmp->ar_name);
			otool(f, 0);
			tmp = (void*)tmp + ft_atoi(tmp->ar_size) + sizeof(struct ar_hdr);
		}
		else
			exit(0);
	}
}
