#include "../includes/ft_ls.h"

int		cmp_lex(t_file *p1, t_file *p2)
{
	int cmpres;
	cmpres = (ft_strcmp(p1->name, p2->name));
	return (cmpres > 0);//ASCENDING ORDER: when A < B THEN cmpres=0, when B > A then cmpres=1 and swap will be done;
}

int		cmp_time(t_file *p1, t_file *p2)
{

	if (p1->stats.st_mtimespec.tv_sec == p2->stats.st_mtimespec.tv_sec)
	{
		if (p1->stats.st_mtimespec.tv_nsec
			== p2->stats.st_mtimespec.tv_nsec)
			return (cmp_lex(p1, p2));
		else
			return (p1->stats.st_mtimespec.tv_nsec
				> p2->stats.st_mtimespec.tv_nsec);//ASC ORDER
	}
	else
		return (p1->stats.st_mtimespec.tv_sec
			> p2->stats.st_mtimespec.tv_sec);//ASC ORDER
}

static void	lst_content_swap(t_list *one, t_list *two)
{
	void *tmp;

	tmp = one->content;
	one->content = two->content;
	two->content = tmp;
}

void		list_sort(t_list *start, int (*f)(t_file *, t_file *))
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
