/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 13:53:53 by aduban            #+#    #+#             */
/*   Updated: 2017/01/20 19:17:41 by aduban           ###   ########.fr       */
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
	struct mach_header	*header;
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
void	nm(void *ptr, char *file, uint32_t file_size, int multiple)
{
	unsigned int		number;

	number = *(int *)ptr;
	if (number == MH_MAGIC_64 || number == MH_CIGAM_64)
	{
		if (number == MH_CIGAM_64)
			set_swap(1);
		else
			set_swap(0);
		if (multiple)
			ft_printf("\n%s:\n", file);
		handle_64(ptr);
	}
	else if (number == FAT_MAGIC || number == FAT_CIGAM)
	{
		if (number == FAT_CIGAM)
			set_swap_fat(1);
		else
			set_swap_fat(0);
		handle_fat(ptr, file_size, file);
	}
	
	else if (number == MH_MAGIC || number == MH_CIGAM)
	{
		if (number == FAT_CIGAM)
			set_swap_fat(1);
		else
			set_swap_fat(0);
		handle_32(ptr);
	}
	else if (!ft_strncmp(ptr, ARMAG, SARMAG))
	{
		handle_archive(ptr, file, file_size);
	}
	else
		ft_putendl_fd("Wrong binary format", 2);
}

char	*get_ptr(char *ptr, char *file, struct stat *buf)
{
	int			fd;

	if ((fd = open(file, O_RDONLY)) < 0)
	{
		ft_putendl_fd("Error opening binary\n", 2);
		return (NULL);
	}
	if (fstat(fd, buf) < 0)
	{
		ft_putendl_fd("fstat failed\n", 2);
		return (NULL);
	}
	if ((ptr = mmap(0, buf->st_size, PROT_READ, MAP_PRIVATE, fd,
					0)) == MAP_FAILED)
	{
		ft_putendl_fd("mmap failed\n", 2);
		return (NULL);
	}
	return (ptr);
}

int		handle_file(char *file, int multiple)
{
	char		*ptr;
	struct stat	buf;

	ptr = NULL;
	ptr = get_ptr(ptr, file, &buf);
	if (!ptr)
		exit(0);
	nm(ptr, file, buf.st_size, multiple);
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
