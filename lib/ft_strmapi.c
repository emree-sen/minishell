/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 09:09:59 by emsen             #+#    #+#             */
/*   Updated: 2023/10/28 09:10:00 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*buf;
	size_t	i;

	buf = ft_strdup(s);
	if (!buf)
		return (NULL);
	i = 0;
	while (buf[i])
	{
		buf[i] = f(i, buf[i]);
		i++;
	}
	return (buf);
}
