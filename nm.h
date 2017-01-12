/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 13:54:08 by aduban            #+#    #+#             */
/*   Updated: 2017/01/12 16:39:25 by aduban           ###   ########.fr       */
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
typedef struct s_sect
{
	char	*name;
	int		i;
	struct s_sect *next;
}				t_sect;


#endif
