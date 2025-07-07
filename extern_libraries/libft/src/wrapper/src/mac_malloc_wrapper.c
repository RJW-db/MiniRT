/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mac_malloc_wrapper.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rjw <rjw@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/26 02:10:48 by rjw           #+#    #+#                 */
/*   Updated: 2025/03/20 21:16:59 by rjw           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE	//	RTLD_NEXT, must be define before the wrapper.h
#include <wrapper.h>
#if defined(__APPLE__)
# if defined(__MACH__)
#  if (MALLOC_WRAP == true) 

void	*malloc(size_t size)
{
	static void	*(*real_malloc)(size_t) = NULL;

	if (real_malloc == NULL)
	{
		real_malloc = dlsym_handler(RTLD_NEXT, "malloc");
		if (real_malloc == NULL)
		{
			return (NULL);
		}
	}
	if (malloc_toggle(RETRIEVE_MALLOC) == OG_MALLOC_ENABLED)
	{
		if (real_malloc == NULL)
		{
			return (NULL);
		}
		return (real_malloc(size));
	}
	else if (malloc_handler(size, NULL, NULL) == false)
	{
		real_malloc = NULL;
		return (NULL);
	}
	return (real_malloc(size));
}
#  endif
# endif
#endif
