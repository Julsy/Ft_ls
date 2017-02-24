/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iiliuk <iiliuk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/23 17:26:14 by iiliuk            #+#    #+#             */
/*   Updated: 2017/02/23 18:14:13 by iiliuk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int			cmp_lex(t_file *p1, t_file *p2, int order)
{
	int cmp_res;

	cmp_res = (ft_strcmp(p1->name, p2->name));
	if (order)
		return (cmp_res < 0 ? 1 : 0);
	else
		return (cmp_res > 0 ? 1 : 0);
}

int			cmp_time(t_file *p1, t_file *p2, int order)
{
	if (p1->stats.st_mtimespec.tv_sec == p2->stats.st_mtimespec.tv_sec)
	{
		if (p1->stats.st_mtimespec.tv_nsec
			== p2->stats.st_mtimespec.tv_nsec)
			return (cmp_lex(p1, p2, order));
		if (order)
			return (p1->stats.st_mtimespec.tv_nsec
				> p2->stats.st_mtimespec.tv_nsec);
		else
			return (p1->stats.st_mtimespec.tv_nsec
				< p2->stats.st_mtimespec.tv_nsec);
	}
	if (order)
		return (p1->stats.st_mtimespec.tv_sec
			> p2->stats.st_mtimespec.tv_sec);
	else
		return (p1->stats.st_mtimespec.tv_sec
			< p2->stats.st_mtimespec.tv_sec);
}

static void	lst_content_swap(t_list *one, t_list *two)
{
	void *tmp;

	tmp = one->content;
	one->content = two->content;
	two->content = tmp;
}

void		list_sort(t_list *start, int (*f)(t_file *, t_file *, int),
																int order)
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
			if (f(left->content, left->next->content, order))
			{
				lst_content_swap(left, left->next);
				swapped = 1;
			}
			left = left->next;
		}
		right = left;
	}
}
