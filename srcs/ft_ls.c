/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iiliuk <iiliuk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/23 13:28:17 by iiliuk            #+#    #+#             */
/*   Updated: 2017/02/23 18:37:02 by iiliuk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

static t_opts	*init_opts(t_opts *opts)
{
	opts->l = 0;
	opts->r = 0;
	opts->a = 0;
	opts->t = 0;
	opts->big_r = 0;
	return (opts);
}

static t_opts	*get_opts(int argc, char **argv, t_opts *opts)
{
	int		i;
	int		j;

	i = -1;
	while (++i < argc)
	{
		j = 0;
		while (argv[i][++j] != '\0' && argv[i][0] == '-')
		{
			if (!ft_strchr("lRart", argv[i][j]))
			{
				ft_putstr_fd("ls: illegal option -- ", 2);
				write(2, &argv[i][j], 1);
				ft_putstr_fd("\nusage: ls [-Ralrt] [file ...]\n", 2);
				exit(1);
			}
			opts->l = (opts->l == 1 || argv[i][j] == 'l') ? 1 : 0;
			opts->a = (opts->a == 1 || argv[i][j] == 'a') ? 1 : 0;
			opts->r = (opts->r == 1 || argv[i][j] == 'r') ? 1 : 0;
			opts->t = (opts->t == 1 || argv[i][j] == 't') ? 1 : 0;
			opts->big_r = (opts->big_r == 1 || argv[i][j] == 'R') ? 1 : 0;
		}
	}
	return (opts);
}

static t_list	*make_dir_list(int argc, char **argv, int *input)
{
	int			i;
	t_list		*files;
	t_file		*tmp;

	i = 0;
	files = NULL;
	tmp = (t_file *)ft_memalloc(sizeof(t_file));
	while (++i < argc)
	{
		if (!file_exists(argv[i]) && !ft_strchr(&argv[i][0], '-'))
		{
			ft_printf("ft_ls: %s: No such file or directory\n", argv[i]);
			*input += 1;
		}
		else if (ft_strchr(&argv[i][0], '-') == 0 &&
		!ft_strequ(argv[i], "./ft_ls"))
		{
			tmp->name = ft_strdup(argv[i]);
			lstat(tmp->name, &(tmp->stats));
			ft_list_add_back(&files, ft_lstnew(tmp, sizeof(t_file)));
			*input += 1;
		}
	}
	free(tmp);
	return (files);
}

static t_list	*scan_dirs(int argc, char **argv, t_opts *opts)
{
	t_list	*files;
	t_file	*tmp;
	int		input;

	input = 0;
	tmp = (t_file *)ft_memalloc(sizeof(t_file));
	files = make_dir_list(argc, argv, &input);
	if (input == 0 && !ft_list_size(files))
	{
		tmp->name = ft_strdup(".");
		lstat(tmp->name, &(tmp->stats));
		ft_list_add_back(&files, ft_lstnew(tmp, sizeof(t_file)));
	}
	else
		list_sort(files, (opts->t) ? cmp_time : cmp_lex, opts->r);
	free(tmp);
	return (files);
}

int				main(int argc, char **argv)
{
	t_list	*files;
	t_opts	*opts;
	int		size;

	opts = (t_opts *)ft_memalloc(sizeof(t_opts));
	opts = get_opts(argc, argv, init_opts(opts));
	files = scan_dirs(argc, argv, opts);
	size = ft_list_size(files);
	display_args(files, opts, size);
	ft_lstdel(&files, ft_lst_free_cont);
	free(opts);
	return (0);
}
