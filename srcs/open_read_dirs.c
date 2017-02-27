/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_read_dirs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iiliuk <iiliuk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/23 17:22:33 by iiliuk            #+#    #+#             */
/*   Updated: 2017/02/27 14:56:56 by iiliuk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

static t_list	*modify_folder_name(t_list *parent, t_list *files)
{
	char	*parent_folder;
	char	*parent_name;
	char	*file_name;

	if (parent)
		parent_name = ft_strdup(((t_file *)parent->content)->name);
	file_name = ft_strdup(((t_file *)files->content)->name);
	if (parent && (!ft_strequ(parent_name, file_name)))
	{
		parent_folder = ft_strjoin(parent_name, file_name);
		file_name = ft_strjoin(parent_folder, "/");
	}
	else
		file_name = ft_strjoin(file_name, "/");
	if (parent && !ft_strequ("./", file_name))
		ft_printf("\n%.*s:\n", ((int)ft_strlen(file_name) - 1), file_name);
	((t_file *)files->content)->name = ft_strdup(file_name);
	if (parent)
		free(parent_name);
	free(parent_folder);
	free(file_name);
	return (files);
}

static t_list	*read_dir(DIR *folder, t_list *files, t_opts *opts)
{
	struct dirent	*file;
	t_file			*entry;
	t_list			*entries;
	struct stat		stats;

	entries = NULL;
	entry = (t_file *)ft_memalloc(sizeof(t_file));
	file = readdir(folder);
	while (file)
	{
		if (opts->a || file->d_name[0] != '.')
		{
			entry->name = ft_strdup(file->d_name);
			lstat(ft_strjoin(((t_file *)files->content)->name,
			file->d_name), &(entry->stats));
			ft_list_add_back(&entries, ft_lstnew(entry, sizeof(t_file)));
		}
		file = readdir(folder);
	}
	free(entry);
	return (entries);
}

t_list			*open_dir(t_list *parent, t_list *files, t_opts *opts)
{
	DIR				*folder;
	t_list			*entries;
	char			*file_name;

	entries = NULL;
	file_name = ft_strdup(((t_file *)files->content)->name);
	files = modify_folder_name(parent, files);
	folder = opendir(((t_file *)files->content)->name);
	if (!folder)
	{
		ft_printf("ft_ls: %s: Permission denied\n", file_name);
		free(file_name);
		return (NULL);
	}
	entries = read_dir(folder, files, opts);
	closedir(folder);
	return (entries);
}
