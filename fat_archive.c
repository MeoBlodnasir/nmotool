/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_archive.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/19 18:23:11 by aduban            #+#    #+#             */
/*   Updated: 2017/02/07 17:19:23 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void			handle_fat(char *ptr, int file_size, char *file)
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
			handle_segv(NULL, 0, ptr + offset);
			nm(ptr + offset, file, file_size, 0);
			return ;
		}
		arch += sizeof(arch) / sizeof(void*);
		i--;
	}
}

struct ar_hdr	*get_arch(char *ptr, int *k)
{
	struct ar_hdr	*arch;
	int				size;

	arch = (void*)ptr + SARMAG;
	handle_segv(NULL, 0, arch);
	size = get_name_size(arch->ar_name);
	if (size == 0)
		*k = 1;
	arch = (void*)ptr + sizeof(*arch) + SARMAG + size;
	size += *((int*)(arch));
	arch = (void*)ptr + sizeof(*arch) + SARMAG + size + sizeof(int);
	size += *((int*)(arch));
	arch = (void*)ptr + sizeof(*arch) + SARMAG + size + (2 * sizeof(int));
	handle_segv(NULL, 0, arch);
	return (arch);
}

void			handle_archive(char *ptr, char *file, uint32_t file_size)
{
	struct ar_hdr	*tmp;
	char			*str;
	int				k;

	k = 0;
	tmp = get_arch(ptr, &k);
	while (tmp < (struct ar_hdr*)(file_size + (void*)ptr))
	{
		if (k == 1)
			str = tmp->ar_name;
		else
			str = (char*)(ft_strstr(tmp->ar_name, ARFMAG) + ft_strlen(ARFMAG));
		if (ft_strcmp(str, SYMDEF) != 0 && ft_strcmp(str, SYMDEF_SORTED) != 0)
		{
			ft_printf("\n%s(%s):\n", file, (char*)(ft_strstr(tmp->ar_name,
							ARFMAG) + ft_strlen(ARFMAG)));
			nm((char*)(tmp + 1) + get_name_size(tmp->ar_name),
					NULL, file_size, 0);
		}
		tmp = (void*)tmp + ft_atoi(tmp->ar_size) + sizeof(struct ar_hdr);
	}
}

void			pre_fat(unsigned int number, char *ptr, int file_size,
		char *file)
{
	if (number == FAT_CIGAM)
		set_swap_fat(1);
	else
		set_swap_fat(0);
	handle_fat(ptr, file_size, file);
}
