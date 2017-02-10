#include "../includes/ft_ls.h"

int		cmp_lex(t_list *p1, t_list *p2)
{
	char* str1;
	char* str2;

	str1 = (char *)p1;
	str2 = (char *)p2;
	return (ft_strcmp(str1, str2));
}

int		cmp_time(t_list *p1, t_list *p2)
{
	struct stat stats;
	char*		str1;
	char*		str2;
	time_t		time1;
	time_t		time2;

	str1 = (char *)p1;
	str2 = (char *)p2;
	stat(str1, &stats);
	time1 = stats.st_mtime;
	stat(str2, &stats);
	time2 = stats.st_mtime;
	if (time1 == time2)
		return (cmp_lex(p1, p2));
	else
		return (time1 < time2);
}

static void	lst_content_swap(t_list *one, t_list *two)
{
	void *tmp;

	tmp = one->content;
	one->content = two->content;
	two->content = tmp;
}

void		list_sort(t_list *start, int (*f)(t_list *, t_list *))
{
	int		swapped;
	t_list	*left;
	t_list	*right;

	right = NULL;
	if (start == NULL)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		left = start;
		while (left->next != right)
		{
			if (f(left->content, left->next->content))
			{
				lst_content_swap(left, left->next);
				swapped = 1;
			}
			left = left->next;
		}
		right = left;
	}
}

// void		sort_files(t_list *files, t_opts opts)
// {
// 	if (opts->t)
// 		list_sort(files, cmp_time);
// }