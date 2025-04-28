/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:18:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/28 15:36:44 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void	make_dir_if_needed(const char *path)
{
	if (access(path, F_OK) != 0)
		mkdir(path, 0755);
}

void	make_tile_dir(char id_char)
{
	char dir[64];
	snprintf(dir, sizeof(dir), "print/%c", id_char);
	make_dir_if_needed(dir);
}

void	writeBMP(const char *filename, const unsigned char *data, int width, int height)
{
	FILE *f;
	int filesize = 54 + 4 * width * height;

	unsigned char bmpfileheader[14] = {
		'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0
	};
	unsigned char bmpinfoheader[40] = {
		40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,32,0
	};

	bmpfileheader[2] = (unsigned char)(filesize    );
	bmpfileheader[3] = (unsigned char)(filesize>> 8);
	bmpfileheader[4] = (unsigned char)(filesize>>16);
	bmpfileheader[5] = (unsigned char)(filesize>>24);

	bmpinfoheader[4] = (unsigned char)(width    );
	bmpinfoheader[5] = (unsigned char)(width>> 8);
	bmpinfoheader[6] = (unsigned char)(width>>16);
	bmpinfoheader[7] = (unsigned char)(width>>24);
	bmpinfoheader[8] = (unsigned char)(height    );
	bmpinfoheader[9] = (unsigned char)(height>> 8);
	bmpinfoheader[10]= (unsigned char)(height>>16);
	bmpinfoheader[11]= (unsigned char)(height>>24);

	f = fopen(filename, "wb");
	if (!f) {
		perror("fopen");
		return;
	}
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	fwrite(data, 4, width * height, f);
	fclose(f);
}

void	print_line(t_data *data, int *map)
{
	printf("\t| ");
	for (int i = 0; i < data->map->wid; i++)
		printf("%c", (char)(*(map + i)));
	printf(" |\n");
}

void	print_map(t_data *data)
{
	int	y;

	y = -1;
	printf("Map is :\n");
	printf("\t.");
	for (int i = 0; i < data->map->wid + 2; i++)
		printf("-");
	printf(".\n");
	printf("\t| %*s |\n", data->map->wid, "");
	while (++y < data->map->len)
		print_line(data, data->map->matrix + y * data->map->wid);
	printf("\t| %*s |\n", data->map->wid, "");
	printf("\t*");
	for (int i = 0; i < data->map->wid + 2; i++)
		printf("-");
	printf("*\n");
}

#include <stdio.h> // Don't forget this for FILE and functions like fopen, fprintf, fclose

void	print_lline(t_data *data, float *map, int fp)
{
	dprintf(fp, "\t| ");
	for (int i = 0; i < data->lmap.wid; i++)
		dprintf(fp, "%0.2f ", (*(map + i)));
	dprintf(fp, "|\n");
}

void	print_lmap(t_data *data)
{
	int fp = open("lmap.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (!fp)
		return;

	int	y = -1;
	dprintf(fp, "Light map is :\n");
	dprintf(fp, "\t.");
	for (int i = 0; i < data->lmap.wid * 5 + 1; i++)
		dprintf(fp, "-");
	dprintf(fp, ".\n");
	dprintf(fp, "\t| %*s |\n", data->lmap.wid * 5 - 1, "");
	while (++y < data->lmap.len)
		print_lline(data, data->lmap.lmap + y * data->lmap.wid, fp);
	dprintf(fp, "\t| %*s |\n", data->lmap.wid * 5 - 1, "");
	dprintf(fp, "\t*");
	for (int i = 0; i < data->lmap.wid * 5 + 1; i++)
		dprintf(fp, "-");
	dprintf(fp, "*\n");
	close(fp); // Always close the file!
}


#define EXPORT_TEXTURE(tex, label, id_char)                                       \
	do {                                                                          \
		if ((tex).img) {                                                          \
			make_tile_dir(id_char);                                               \
			char path[256];                                                       \
			snprintf(path, sizeof(path), "print/%c/%s.bmp", id_char, label);      \
			writeBMP(path, (const unsigned char *)(tex).data,                     \
			         (tex).width, (tex).height);                                  \
		}                                                                         \
	} while (0)

void	print_dict(t_data *data)
{
	size_t	i;
	t_tile	*tile;

	make_dir_if_needed("print");
	i = -1;
	printf("Tiles in map are :\n");
	while (++i < 256)
	{
		tile = get_tile_dict()[i];
		if (tile)
		{
			printf("\tId : %c\n", (char)i);
			EXPORT_TEXTURE(*tile->tex_no.img, "tex_no", (char)i);
			EXPORT_TEXTURE(*tile->tex_so.img, "tex_so", (char)i);
			EXPORT_TEXTURE(*tile->tex_we.img, "tex_we", (char)i);
			EXPORT_TEXTURE(*tile->tex_ea.img, "tex_ea", (char)i);
			EXPORT_TEXTURE(*tile->tex_ce.img, "tex_ce", (char)i);
			EXPORT_TEXTURE(*tile->tex_fl.img, "tex_fl", (char)i);
		}
	}
	printf("\nYou can look them up at print/id/name_of_texture.bmp !\n\n");
	print_map(data);
	print_lmap(data);
}
