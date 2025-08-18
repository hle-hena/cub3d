/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   full_screen.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:52:29 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/17 14:58:23 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// void	set_fullscreen(void *mlx_ptr, void *win_ptr);

// void	set_fullscreen(t_xvar *xvar, t_win_list *win)
// {
// 	Atom	wm_state;
// 	Atom	fullscreen;
// 	XEvent	xev;

// 	fullscreen = XInternAtom(xvar->display, "_NET_WM_STATE_FULLSCREEN", False);	
// 	wm_state = XInternAtom(xvar->display, "_NET_WM_STATE", False);
// 	xev = (XEvent){0};
// 	xev.type = ClientMessage;
// 	xev.xclient.window = win->window;
// 	xev.xclient.message_type = wm_state;
// 	xev.xclient.format = 32;
// 	xev.xclient.data.l[0] = 1;
// 	xev.xclient.data.l[1] = fullscreen;
// 	xev.xclient.data.l[2] = 0;
// 	xev.xclient.data.l[3] = 1;
// 	xev.xclient.data.l[4] = 0;

// 	XSendEvent(
// 		xvar->display,
// 		DefaultRootWindow(xvar->display),
// 		False,
// 		SubstructureNotifyMask | SubstructureRedirectMask,
// 		&xev
// 	);
// }