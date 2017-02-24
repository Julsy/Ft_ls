void		display_entries(t_list *parent, t_list *files, t_opts *opts)
{
	t_list			*entries;
	t_list			*tmp_entries;
	struct stat		stats;
	int				width[6] = {0, 0, 0, 0, 0};
	int				spec;
	
	if (!(entries = open_dir(parent, files, opts)))
		return ;
	if (opts->l)
		print_total(entries);
	list_sort(entries, (opts->t) ? cmp_time : cmp_lex, opts->r);
	tmp_entries = entries;
	spec = get_width_if_spec(entries, width);
	while (entries)
	{
		display_stats((t_file *)entries->content, ((t_file *)files->content), opts, width, spec);
		entries = entries->next;
	}
	entries = tmp_entries;
	if (opts->R)
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