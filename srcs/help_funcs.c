/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iiliuk <iiliuk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/23 17:15:58 by iiliuk            #+#    #+#             */
/*   Updated: 2017/02/27 14:53:58 by iiliuk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int		file_exists(char *filename)
{
	struct stat		buffer;

	return (lstat(filename, &buffer) == 0);
}

void	print_total(t_list *folder)
{
	blkcnt_t blocks;

	blocks = 0;
	while (folder)
	{
		blocks += ((t_file *)folder->content)->stats.st_blocks;
		folder = folder->next;
	}
	ft_printf("total %zu\n", (blocks));
}

void	get_width_and_spec(t_list *files, int w[7])
{
	t_file *tmp;

	ft_bzero(w, sizeof(int) * 7);
	while (files)
	{
		tmp = (t_file *)files->content;
		w[0] = MAX(ft_strlen_num(tmp->stats.st_nlink), w[0]);
		if (getpwuid(tmp->stats.st_uid))
			w[1] = MAX(ft_strlen(getpwuid(tmp->stats.st_uid)->pw_name), w[1]);
		else
			w[1] = MAX(ft_strlen(ft_itoa(tmp->stats.st_uid)), w[1]);
		if (getgrgid(tmp->stats.st_gid))
			w[2] = MAX(ft_strlen(getgrgid(tmp->stats.st_gid)->gr_name), w[2]);
		else
			w[2] = MAX(ft_strlen(ft_itoa(tmp->stats.st_gid)), w[2]);
		w[3] = MAX(ft_strlen_num(tmp->stats.st_size), w[3]);
		w[4] = MAX(ft_strlen_num(tmp->stats.st_rdev >> 24), w[4]);
		w[5] = MAX(ft_strlen_num(tmp->stats.st_rdev & 0xFFFFFF), w[5]);
		w[6] = (((tmp->stats.st_mode & S_IFMT) == S_IFBLK) ||
		((tmp->stats.st_mode & S_IFMT) == S_IFCHR) ? 1 : 0);
		files = files->next;
	}
	w[0]++;
}
