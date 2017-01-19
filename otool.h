/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 13:54:08 by aduban            #+#    #+#             */
/*   Updated: 2017/01/19 17:57:31 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdlib.h>
# include "libft/libft.h"
# include <mach-o/ranlib.h>
# include <mach-o/fat.h>
# include <ar.h>

typedef struct			s_elem
{
	long int			value;
	char				type;
	char				*str;
	struct s_elem		*next;
	struct s_elem		*prev;
}						t_elem;
typedef struct			s_sect
{
	char				*name;
	int					i;
	struct s_sect		*next;
}						t_sect;

typedef struct			s_offlist
{
	uint32_t			off;
	uint32_t			strx;
	struct s_offlist	*next;
}						t_offlist;
typedef struct			s_norm
{
	int					ncmds;
	int					i;
}						t_norm;
typedef struct			s_file
{
	char				*ptr;
	char				*file;
	uint32_t			size;
}						t_file;

void					nm(t_file f, int multiple, int ch);

#endif
