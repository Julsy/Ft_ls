/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iiliuk <iiliuk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/23 17:24:33 by iiliuk            #+#    #+#             */
/*   Updated: 2017/02/23 18:44:08 by iiliuk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

static void			display_entries(t_list *parent, t_list *files, t_opts *opts)
{
	t_list	*entries;
	t_list	*tmp_entries;
	int		width[7];
	
	if (!(entries = open_dir(parent, files, opts)))
		return ;
	if (opts->l)
		print_total(entries);
	list_sort(entries, (opts->t) ? cmp_time : cmp_lex, opts->r);
	tmp_entries = entries;
	get_width_and_spec(entries, width);
	while (entries)
	{
		if (!opts->l)
			ft_printf("%s\n", ((t_file *)entries->content)->name);
		else
			display_stats((t_file *)entries->content, ((t_file *)files->content),
			opts, width);
		entries = entries->next;
	}
	entries = tmp_entries;
	if (opts->big_r)
		do_recursion(entries, files, opts);
}

void			do_recursion(t_list	*entries, t_list *files, t_opts *opts)
{
	while (entries)
	{
		if (S_ISDIR(((t_file *)entries->content)->stats.st_mode) &&
		!ft_strequ(((t_file *)entries->content)->name, ".") &&
		!ft_strequ(((t_file *)entries->content)->name, ".."))
			display_entries(files, entries, opts);
		entries = entries->next;
	}
	ft_lstdel(&entries, ft_lst_free_cont);
}

void			display_args(t_list	*files, t_opts *opts, int size)
{
	t_list	*prev;
	int		width[7];

	prev = NULL;
	get_width_and_spec(files, width);
	while (files)
	{
		if (S_ISDIR(((t_file *)files->content)->stats.st_mode))
		{
			if (!ft_strequ(".", ((t_file *)files->content)->name))
			{
				if (prev)
					ft_printf("\n");
				if (size > 1)
					ft_printf("%s:\n", ((t_file *)files->content)->name);
			}
			display_entries(NULL, files, opts);
		}
		else
		{
			if (!opts->l)
				ft_printf("%s\n", ((t_file *)files->content)->name);
			else
				display_stats((t_file *)files->content, NULL, opts, width);
		}
		prev = files;
		files = files->next;
	}
}
