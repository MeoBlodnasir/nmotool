/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 13:54:08 by aduban            #+#    #+#             */
/*   Updated: 2017/01/23 16:24:51 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OTOOL_H
# define OTOOL_H
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

void					otool(t_file f, int ch);
char					*get_ptr(char *ptr, char *file, struct stat *buf);
int						get_name_size(char *name);
uint32_t				swap(uint32_t val);
void					print_otool(struct section_64 *sec, char *ptr);
void					handle_64(char *ptr);
void					handle_32(char *ptr);
int						set_swap_fat(int i);
int						set_swap(int i);
uint32_t				swap_32(uint32_t i);
int64_t					swap_64(int64_t i);
uint32_t				swap_32_fat(uint32_t i);
int64_t					swap_64_fat(int64_t i);
void					handle_32(char *ptr);
void					print_otool_32(struct section *sec, char *ptr);
void					handle_archive(char *ptr, char *file,
		uint32_t file_size);
void					handle_fat(t_file f);
int						set_swap(int i);
int						set_swap_fat(int i);
uint32_t				swap(uint32_t val);
uint32_t				swap_32(uint32_t i);
int64_t					swap_endian_64(int64_t i);

#endif
