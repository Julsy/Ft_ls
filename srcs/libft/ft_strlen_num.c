/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_num.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iiliuk <iiliuk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/21 15:42:51 by iiliuk            #+#    #+#             */
/*   Updated: 2017/02/21 15:42:54 by iiliuk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_strlen_num(int x)
{
	if (x < 10)
		return (1);
	else
		return (ft_strlen_num(x / 10) + 1);
}
