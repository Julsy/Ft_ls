#include "../includes/ft_ls.h"

const char*			current_dir = ".";
/*
**  Program works as follows(not including handling errors):
**  - Read arguments and sort them.
**  - For the list of received files execute ft_ls.
**  - If there is only one file that isn't a directory, display that file.
**  - If there is one file that is a directory, pass entries to ft_ls.
**  - If there are many files, pass all to ft_ls.
**  - ft_ls:
**       1. If -a not specified and file isn't specified in the
**          command line, exclude ".*" files.
**       2. Gather info about each file and format it according to options.
**       3. Sort entries either by time (if -t specified) or
**          lexicographically by name.
**       4. If -r specified, reverse entries.
**       5. If a file isn't specified in the command line or
**          there are multiple file arguments, display name of file.// ?
**       6. Print formatted entries of the list. If file is a directory and
**          is specified in the command line, don't display it.
**       7. If -R specified or it is directory specified in the command line:
**             ~ For each directory collect entries.
**             ~ For each directory execute ft_ls for each entry
*/

t_opts		*get_opts(int argc, char **argv)
{
	int		i;
	int		j;
	t_opts	*opts;

	i = -1;
	opts = (t_opts *)ft_memalloc(sizeof(t_opts));
	while (++i < argc)
	{
		j = 0;
		while (argv[i][++j] != '\0' && argv[i][0] == '-')
		{
			if (argv[i][j] == 'l')
				opts->l = 1;
			else if (argv[i][j] == 'R')
				opts->R = 1;
			else if (argv[i][j] == 'a')
				opts->a = 1;
			else if (argv[i][j] == 'r')
				opts->r = 1;
			else if (argv[i][j] == 't')
				opts->t = 1;
			else if (argv[i][j] != 'l' && argv[i][j] != 'R'&& argv[i][j] != 'a' &&
			argv[i][j] != 'r' && argv[i][j] != 't') 
			{
				ft_putstr_fd("ls: illegal option -- ", 2);
				write(2, &argv[i][j], 1);
				ft_putstr_fd("\nusage: ls [-Ralrt] [file ...]\n", 2);
				exit(1);
			}
		}
	}
	return (opts);
}

t_list		*modify_folder_name(t_list *parent, t_list *files)
{
	char	*parent_folder;

	if (parent && (!ft_strequ(((t_file *)parent->content)->name, ((t_file *)files->content)->name)))
	{
		parent_folder = ft_strjoin(((t_file *)parent->content)->name, ((t_file *)files->content)->name);
		((t_file *)files->content)->name = ft_strjoin(parent_folder, "/");
	}
	else
		((t_file *)files->content)->name = ft_strjoin(((t_file *)files->content)->name, "/");
	if (!ft_strequ("./", ((t_file *)files->content)->name) && parent)
		ft_printf("\n%.*s:\n", ((int)ft_strlen(((t_file *)files->content)->name)) - 1,
	((t_file *)files->content)->name);
	return (files);
}

t_list		*read_dir(t_list *parent, t_list *files, t_opts *opts)
{
	DIR				*folder;
	struct dirent	*file;
	t_file			*entry;
	t_list			*entries;
	struct stat		stats;

	entries = NULL;
	files = modify_folder_name(parent, files);
	folder = opendir(((t_file *)files->content)->name);
	entry = (t_file *)ft_memalloc(sizeof(t_file));
	file = readdir(folder);
	while (file)
	{
		if(opts->a || file->d_name[0] != '.')
		{
			entry->name = ft_strdup(file->d_name);
			lstat(ft_strjoin(((t_file *)files->content)->name, file->d_name), &(entry->stats));
			ft_list_add_back(&entries, ft_lstnew(entry, sizeof(t_file)));			
		}
		file = readdir(folder);
	}
	free(entry);
	closedir(folder);
	return (entries);
}

void		display_entries(t_list *parent, t_list *files, t_opts *opts)
{
	t_list			*entries;
	t_list			*tmp_entries;
	struct stat		stats;

	entries = read_dir(parent, files, opts);
	if (opts->l)
		print_total(entries);
	list_sort(entries, (opts->t) ? cmp_time : cmp_lex, opts->r);
	tmp_entries = entries;
	while (entries)
	{
		display_stats((t_file *)entries->content, (parent ? (t_file *)parent->content : ((t_file *)files->content)), opts);
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
	free(entries);
}

void	print_list(t_list *list)
{
	if (list)
	{
		printf("content: %s\n", ((t_file *)list->content)->name);
		print_list(list->next);
	}
}

static t_list	*scan_dirs(int argc, char **argv, t_opts *opts)
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
			ft_printf("ft_ls: %s: No such file or directory\n", argv[i]);
		else if (ft_strchr(&argv[i][0], '-') == 0 && !ft_strequ(argv[i], "./ft_ls"))
		{
			tmp->name = ft_strdup(argv[i]);
			lstat(tmp->name, &(tmp->stats));
			ft_list_add_back(&files, ft_lstnew(tmp, sizeof(t_file)));
		}
	}
	if (!ft_list_size(files) && (!argv[1] || file_exists(".")))
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
	t_list *files;
	t_opts *opts;

	opts = get_opts(argc, argv);
	files = scan_dirs(argc, argv, opts);
	while (files)
	{
		if (S_ISDIR(((t_file *)files->content)->stats.st_mode))
			display_entries(NULL, files, opts);
		else
			display_stats((t_file *)files->content, NULL, opts);
		files = files->next;
	}
	free(opts);
	return (0);
}