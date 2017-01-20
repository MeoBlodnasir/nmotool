/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 13:53:53 by aduban            #+#    #+#             */
/*   Updated: 2017/01/20 19:42:55 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

void	handle_fat(t_file f)
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
			nm(f, 0, 0);
			return ;
		}
		arch += sizeof(arch) / sizeof(void*);
		i--;
	}
}

void	handle_archive(char *ptr, char *file, uint32_t file_size)
{
	struct ar_hdr	*arch;
	t_offlist		*lst;
	int				size;
	struct ar_hdr	*tmp;
	t_file			f;

	lst = NULL;
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
			ft_printf("%s(%s):\n", file, (char*)(ft_strstr(tmp->ar_name,
							ARFMAG) + ft_strlen(ARFMAG)));
			f.ptr = (char*)(tmp + 1) + get_name_size(tmp->ar_name);
			nm(f, 0, 0);
			tmp = (void*)tmp + ft_atoi(tmp->ar_size) + sizeof(struct ar_hdr);
		}
		else
			exit(0);
	}
}

void	nm(t_file f, int multiple, int ch)
{
	unsigned int		number;

	number = *(int *)f.ptr;
	(void)multiple;
	if (number == MH_MAGIC_64 || number == MH_CIGAM_64)
	{
		if (number == MH_CIGAM_64)
			set_swap(1);
		else
			set_swap(0);
		if (ch)
			ft_printf("%s:\n", f.file);
		ft_printf("Contents of (__TEXT,__text) section\n");
		handle_64(f.ptr);
	}
	else if (number == FAT_MAGIC || number == FAT_CIGAM)
	{
		if (number == FAT_CIGAM)
			set_swap_fat(1);
		else
			set_swap_fat(0);
		if (ch)
			ft_printf("%s:\n", f.file);
		ft_printf("Contents of (__TEXT,__text) section\n");
		handle_fat(f);
	}
	else if (number == MH_MAGIC || number == MH_CIGAM)
	{
		if (number == FAT_CIGAM)
			set_swap_fat(1);
		else
			set_swap_fat(0);
		if (ch)
			ft_printf("%s:\n", f.file);
		ft_printf("Contents of (__TEXT,__text) section\n");
		handle_32(f.ptr);
	}
	else if (!ft_strncmp(f.ptr, ARMAG, SARMAG))
	{
		ft_printf("Archive : %s\n", f.file);
		handle_archive(f.ptr, f.file, f.size);
	}
	else
		ft_putendl_fd("Wrong binary format", 2);
}

int		handle_file(char *file, int multiple)
{
	char		*ptr;
	struct stat	buf;
	t_file		f;

	ptr = NULL;
	ptr = get_ptr(ptr, file, &buf);
	if (!ptr)
		exit(0);
	f.ptr = ptr;
	f.file = file;
	f.size = buf.st_size;
	nm(f, multiple, 1);
	if (munmap(ptr, buf.st_size) < 0)
	{
		ft_printf("unmap failed\n");
		exit(-1);
	}
	return (0);
}

int		main(int ac, char **av)
{
	char	*file;
	int		multiple;
	int		i;

	multiple = 0;
	if (ac < 2)
	{
		file = ft_strdup("a.out");
		handle_file(file, multiple);
	}
	else
	{
		i = 1;
		if (ac > 2)
			multiple = 1;
		while (i < ac)
		{
			handle_file(av[i], multiple);
			i++;
		}
	}
}
