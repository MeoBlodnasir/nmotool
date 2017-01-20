/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/19 18:29:32 by aduban            #+#    #+#             */
/*   Updated: 2017/01/20 19:08:43 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

void		print_otool(struct section_64 *sec, char *ptr)
{
	void	*start;
	void	*end;
	void	*p;
	int		i;

	start = (void*)ptr + sec->offset;
	end = start + sec->size;
	p = start;
	i = 0;
	while (p < end)
	{
		ft_printf("%016llx\t", sec->addr + (p - start));
		i = -1;
		while (++i < 16 && p < end)
		{
			ft_printf("%02hhx ", *(char*)p);
			++p;
		}
		ft_printf("\n");
	}
}

int			get_name_size(char *name)
{
	int size;

	size = ft_atoi(ft_strchr(name, '/') + 1);
	return (size);
}

char		*get_ptr(char *ptr, char *file, struct stat *buf)
{
	int			fd;

	if ((fd = open(file, O_RDONLY)) < 0)
	{
		ft_printf("Error opening binary\n");
		return (NULL);
	}
	if (fstat(fd, buf) < 0)
	{
		ft_printf("fstat failed\n");
		return (NULL);
	}
	if ((ptr = mmap(0, buf->st_size, PROT_READ, MAP_PRIVATE, fd,
					0)) == MAP_FAILED)
	{
		ft_printf("mmap failed\n");
		return (NULL);
	}
	return (ptr);
}

uint32_t	swap(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}


int64_t		swap_endian_64(int64_t i)
{
	return ((i >> 0x38) & 0x00000000000000ffLL)
		| ((i >> 0x28) & 0x000000000000ff00LL)
		| ((i >> 0x18) & 0x0000000000ff0000LL)
		| ((i >> 0x08) & 0x00000000ff000000LL)
		| ((i << 0x08) & 0x000000ff00000000LL)
		| ((i << 0x18) & 0x0000ff0000000000LL)
		| ((i << 0x28) & 0x00ff000000000000LL)
		| ((i << 0x38) & 0xff00000000000000LL);
}

int	set_swap(int i)
{
	static int s = 0;
	if (i == 0)
		s = 0;
	else if (i == 1)
		s = 1;
	return (s);
}
uint32_t		swap_32(uint32_t i)
{
	int g;
	g = set_swap(-1);
	if (g == 1)
		return (swap(i));
	return (i);
}

int64_t		swap_64(int64_t i)
{
	int g;
	g = set_swap(-1);
	if (g == 1)
		return (swap_endian_64(i));
	return (i);
}


int	set_swap_fat(int i)
{
	static int s = 0;
	if (i == 0)
		s = 0;
	else if (i == 1)
		s = 1;
	return (s);
}
uint32_t		swap_32_fat(uint32_t i)
{
	int g;
	g = set_swap_fat(-1);
	if (g == 1)
		return (swap(i));
	return (i);
}

int64_t		swap_64_fat(int64_t i)
{
	int g;
	g = set_swap_fat(-1);
	if (g == 1)
		return (swap_endian_64(i));
	return (i);
}
