/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:04:42 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 12:56:57 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline t_uint64	get_time_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((t_uint64)tv.tv_sec * 1000000 + tv.tv_usec);
}

void	fps_counter(t_data	*data)
{
	static t_uint64	last_frame_time = 0;
	t_uint64		now;
	t_int64			elapsed;

	now = get_time_us();
	if (last_frame_time == 0)
		last_frame_time = now;
	elapsed = now - last_frame_time;
	data->delta_t = (float)elapsed / FRAME_TIME_US;
	printf("\rFPS : %.2f", (float)1000000.0 / elapsed);
	fflush(stdout);
	last_frame_time = get_time_us();
}
