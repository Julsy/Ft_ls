#include "../includes/ft_ls.h"

static void		print_permissions(mode_t mode)
{
	ft_putchar((mode & S_IRUSR) ? 'r' : '-');
	ft_putchar((mode & S_IWUSR) ? 'w' : '-');
	if (mode & S_ISUID)
		ft_putchar((mode & S_IXUSR) ? 's' : 'S');
	else
		ft_putchar((mode & S_IXUSR) ? 'x' : '-');
	ft_putchar((mode & S_IRGRP) ? 'r' : '-');
	ft_putchar((mode & S_IWGRP) ? 'w' : '-');
	if (mode & S_ISGID)
		ft_putchar((mode & S_IXGRP) ? 's' : 'S');
	else
		ft_putchar((mode & S_IXGRP) ? 'x' : '-');
	ft_putchar((mode & S_IROTH) ? 'r' : '-');
	ft_putchar((mode & S_IWOTH) ? 'w' : '-');
	if (mode & S_ISVTX)
		ft_putchar((mode & S_IXUSR) ? 't' : 'T');
	else
		ft_putchar((mode & S_IXOTH) ? 'x' : '-');
}

static void		print_filetype(mode_t mode)
{
	if ((mode & S_IFMT) == S_IFREG)
		ft_putchar('-');
	else if ((mode & S_IFMT) == S_IFDIR)
		ft_putchar('d');
	else if ((mode & S_IFMT) == S_IFLNK)
		ft_putchar('l');
	else if ((mode & S_IFMT) == S_IFCHR)
		ft_putchar('c');
	else if ((mode & S_IFMT) == S_IFBLK)
		ft_putchar('b');
	else if ((mode & S_IFMT) == S_IFSOCK)
		ft_putchar('s');
	else if ((mode & S_IFMT) == S_IFIFO)
		ft_putchar('f');
}

static void		print_name_or_link(t_file *file, t_file *parent, mode_t mode)
{
	char		link_buf[10024];
	ssize_t		count;
	char		*path;

	if ((mode & S_IFMT) == S_IFLNK)
	{
		ft_bzero(link_buf, sizeof(link_buf));
		if (parent)
		{
			path = ft_strjoin(parent->name, "/");
			path = ft_strjoin(path, file->name);
		}
		else
			path = ft_strdup(file->name);

		count = readlink(path, link_buf, sizeof(link_buf));
		if (count < 0)
		{
			free(path);
			perror("readlink");
			exit(EX_IOERR);
		}
		link_buf[count] = '\0';
		ft_printf(" %s -> %s\n", file->name, link_buf);
		free(path);
	}
	else
		ft_printf(" %s\n", file->name);
}

static void		print_time(time_t mod_time)
{
	char	*str;
	char	*end;
	char	*start;
	time_t	ct;

	ct = time(NULL);
	if ((str = ctime(&mod_time)) == NULL)
		return ;
	start = str + 4;
	end = str + 10;
	*end = 0;
	ft_putstr(start);
	ft_putchar(' ');
	if (mod_time > ct || mod_time + SIX_MONTHS_AGO < ct)
	{
		start = str + 20;
		end = str + 24, ft_putchar(' ');
	}
	else
	{
		start = str + 11;
		end = str + 16;
	}
	*end = 0;
	ft_putstr(start);
}

void			print_total(t_list *folder)
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

int				get_width_if_spec(t_list *files, int w[6])
{
	int	special;
	t_file *tmp;

	special = 0;
	w[0] = 0;
	w[1] = 0;
	w[2] = 0;
	w[3] = 0;
	w[4] = 0;
	w[5] = 0;
	while (files)
	{
		tmp = (t_file *)files->content;
		w[0] = MAX(ft_strlen_num(tmp->stats.st_nlink), w[0]);
		if (getpwuid(tmp->stats.st_uid))
			w[1] = MAX(ft_strlen(getpwuid(tmp->stats.st_uid)->pw_name), w[1]);
		else
			w[1]=MAX(ft_strlen(ft_itoa(tmp->stats.st_uid)), w[1]);
		if (getgrgid(tmp->stats.st_gid))
			w[2] = MAX(ft_strlen(getgrgid(tmp->stats.st_gid)->gr_name), w[2]);
		else
			w[2] = MAX(ft_strlen(ft_itoa(tmp->stats.st_gid)), w[2]);
		w[3] = MAX(ft_strlen_num(tmp->stats.st_size), w[3]);
		w[4] = MAX(ft_strlen_num(tmp->stats.st_rdev >> 24), w[4]);
		w[5] = MAX(ft_strlen_num(tmp->stats.st_rdev & 0xFFFFFF), w[5]);
		special = (((tmp->stats.st_mode & S_IFMT) == S_IFBLK) ||
		((tmp->stats.st_mode & S_IFMT) == S_IFCHR));
		files = files->next;
	}
	w[0]++;
	return (special);
}

void			display_stats(t_file *file, t_file *parent, t_opts *opts, int width[6], int spec)
{
	if (opts->l == 0)
	{
		ft_printf("%s\n", file->name);
		return ;
	}
	//printf("%zd, %zd, %zd, %zd, %zd, %d\n", width[0], width[1], width[2], width[3], width[4], spec);
	print_filetype(file->stats.st_mode);
	print_permissions(file->stats.st_mode);
	ft_printf(" %*d ", width[0], file->stats.st_nlink);
	if (getpwuid(file->stats.st_uid))
		ft_printf(spec ? "%-*s " : "%*s ", width[1], getpwuid(file->stats.st_uid)->pw_name);
	else
		ft_printf(spec ? "%-*s " : "%*s ", width[1], ft_itoa(file->stats.st_uid));
	if (getgrgid(file->stats.st_gid))
		ft_printf(spec ? " %-*s " : " %*s ", width[2], getgrgid(file->stats.st_gid)->gr_name);
	else
		ft_printf(spec ? " %-*s " : " %*s ", width[2], ft_itoa(file->stats.st_gid));
	if (spec)
	{
		if ((file->stats.st_mode & S_IFMT) == S_IFLNK)
			ft_printf("  %*s ", width[4], "");
		else
			ft_printf("  %*d,", width[4], file->stats.st_rdev >> 24);
		ft_printf(" %*d ", width[5], file->stats.st_rdev & 0xFFFFFF);
	}
	else
		ft_printf(" %*lld ", width[3], file->stats.st_size);
	print_time(file->stats.st_mtime);
	print_name_or_link(file, parent, file->stats.st_mode);
}
