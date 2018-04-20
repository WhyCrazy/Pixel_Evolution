#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "include/my.h"

framebuffer_t *framebuffer_create(unsigned int width, unsigned int height)
{
	framebuffer_t *ret;
	int i = 0;

	ret = malloc(sizeof(struct framebuffer));
	ret->width = width;
	ret->height = height;
	ret->pixels = malloc(4 * sizeof(sfUint8) * width * height);
	while (i < 4 * width * height)
		ret->pixels[i++] = 0;
	return (ret);
}

window_t *window_create(void)
{
	window_t *window = malloc(sizeof (window_t));
	sfTexture *texture = sfTexture_create(1920, 1080);
	sfSprite *sprite = sfSprite_create();
	window->sprite = sprite;
	window->texture = texture;
	return (window);
}

void destroy_all(sfRenderWindow *window, window_t *elements, framebuffer_t *pixel)
{
	free(pixel->pixels);
	free(elements->sprite);
	free(elements->texture);
	free(pixel);
	free(elements);
	sfRenderWindow_close(window);
}

void display_framebuffer(framebuffer_t *pixel, window_t *window, sfRenderWindow *display)
{
	sfTexture_updateFromPixels(window->texture, pixel->pixels, 1920, 1080, 0, 0);
	sfSprite_setTexture(window->sprite, window->texture, sfFalse);
	sfRenderWindow_drawSprite(display, window->sprite, NULL);
	sfRenderWindow_display(display);
}

void	my_putpixel(framebuffer_t *buffer, unsigned int width, unsigned int height, sfColor color)
{
	buffer->pixels[4 * (width + height * 1920)] = color.r;
	buffer->pixels[4 * (width + height * 1920) + 1] = color.g;
	buffer->pixels[4 * (width + height * 1920) + 2] = color.b;
	buffer->pixels[4 * (width + height * 1920) + 3] = color.a;
}

void draw_rect(framebuffer_t *pixel, int x, int y, int a, int b, sfColor color)
{
	int tempx = x;
	int tempy = y;

	while (tempy < b) {
		while (tempx < a)
			my_putpixel(pixel, tempx++, tempy, color);
		tempy++;
		tempx = x;
	}
}

int power(int a, int p)
{
	int i = 0;
	int sum = 1;

	while (i < p) {
		sum *= a;
		i++;
	}
	return (sum);
}

void grid(framebuffer_t *pixel, square_t **coor)
{
	int x = 0;
	int y = 0;
	int i = 0;
	int j = 0;
	int red = 0;
	int blue = 0;
	int green = 0;
	int r = 0;
	sfColor color;

	while (x < 95) {
		while (y < 53) {
			i = x + rand() % 3 - 1;
			j = y + rand() % 3 - 1;
			i = (i >= 0 ? i : 0);
			j = (j >= 0 ? j : 0);
			i = (i <= 96 ? i : 96);
			j = (j <= 54 ? j : 54);
			color = sfColor_fromInteger(coor[x][y].color);
			if (coor[x][y].life > 0 && coor[i][j].life <= 0) {
				color = sfColor_fromInteger(coor[x][y].color);
				red = rand() % 11 - 5;
				green = rand() % 11 - 5;
				blue = rand() % 11 - 5;
				color.r += (color.r + red > 0 && color.r + red < 255 ? red : 0);
				color.g += (color.g + green > 0 && color.g + green < 255 ? green : 0);
				color.b += (color.b + blue > 0 && color.b + blue < 255 ? blue : 0);
				color.a = 255;
				r = rand() % 3;
				if (r == 0)
					coor[i][j].life = power(color.r, 1) * power((2 * color.r / ((color.g + color.b) + 1)), 3);
				if (r == 1)
					coor[i][j].life = power(color.g, 1) * power((2 * color.g / ((color.r + color.b) + 1)), 3);
				if (r == 2)
					coor[i][j].life = power(color.b, 1) * power((2 * color.b / ((color.g + color.r) + 1)), 3);
				coor[i][j].color = sfColor_toInteger(color);
			}
			color = sfColor_fromInteger(coor[x][y].color);
			draw_rect(pixel, (x * 20), (y * 20), (x * 20) + 20, (y * 20) + 20, color);
			y++;
		}
		x++;
		y = 0;
	}
	x = 0;
	y = 0;
	while (x < 95) {
		while (y < 53) {
			color = sfColor_fromInteger(coor[x][y].color);
			if (coor[x][y].life > 0)
				coor[x][y].life -= 10;
	//		if (coor[x][y].life <= 0)
	//			color = sfBlack;
			draw_rect(pixel, (x * 20), (y * 20), (x * 20) + 20, (y * 20) + 20, color);
			y++;
		}
		y = 0;
		x++;
	}
}

int main(int argc, const char *argv[])
{
	int i = 0;
	sfRenderWindow *window;
	sfVideoMode mode = {1920, 1080, 32};
	sfEvent event;
	sfColor color = sfWhite;
	sfTexture *texture = sfTexture_create(1920, 1080);
	sfSprite *sprite = sfSprite_create();
	framebuffer_t *pixel = framebuffer_create(1920, 1080);
	window_t *window_elements = window_create();
	sfClock *clock = sfClock_create();
	square_t **coor = malloc(sizeof (square_t*) * 96);
	int j = 0;

	srand(time(NULL));
	while (i < 96) {
		coor[i] = malloc(sizeof (square_t) * 54);
		while (j < 54) {
			coor[i][j].life = 0;
			coor[i][j].color = 0;
			j++;
		}
		j = 0;
		i++;
	}
	i = rand() % 96;
	j = rand() % 54;
	coor[i][j].color = sfColor_toInteger(sfWhite);
	coor[i][j].life = 100;
	window = sfRenderWindow_create(mode, "SFML", sfFullscreen, NULL);
	while (sfRenderWindow_isOpen(window)) {
		while (sfClock_getElapsedTime(clock).microseconds < 10);
		sfClock_restart(clock);
		grid(pixel, coor);
		display_framebuffer(pixel, window_elements, window);
	}
	return (0);
}
