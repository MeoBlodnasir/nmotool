/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/19 18:29:32 by aduban            #+#    #+#             */
/*   Updated: 2017/01/19 18:33:30 by aduban           ###   ########.fr       */
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

uint32_t	swap(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
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
