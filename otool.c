/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 13:53:53 by aduban            #+#    #+#             */
/*   Updated: 2017/01/23 16:37:07 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

void	pre_64(unsigned int number, int ch, t_file f)
{
	if (number == MH_CIGAM_64)
		set_swap(1);
	else
		set_swap(0);
	if (ch)
		ft_printf("%s:\n", f.file);
	handle_64(f.ptr);
}

void	pre_fat(unsigned int number, int ch, t_file f)
{
	if (number == FAT_CIGAM)
		set_swap_fat(1);
	else
		set_swap_fat(0);
	if (ch)
		ft_printf("%s:\n", f.file);
	handle_fat(f);
}

void	otool(t_file f, int ch)
{
	unsigned int		number;

	number = *(int *)f.ptr;
	if (number == MH_MAGIC_64 || number == MH_CIGAM_64)
		pre_64(number, ch, f);
	else if (number == FAT_MAGIC || number == FAT_CIGAM)
		pre_fat(number, ch, f);
	else if (number == MH_MAGIC || number == MH_CIGAM)
	{
		if (number == FAT_CIGAM)
			set_swap_fat(1);
		else
			set_swap_fat(0);
		if (ch)
			ft_printf("%s:\n", f.file);
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

int		handle_file(char *file)
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
	otool(f, 1);
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
		handle_file(file);
	}
	else
	{
		i = 1;
		if (ac > 2)
			multiple = 1;
		while (i < ac)
		{
			handle_file(av[i]);
			i++;
		}
	}
}
