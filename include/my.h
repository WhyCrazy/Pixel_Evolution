#ifndef PIXEL_H_
# define PIXEL_H_
#define MAX_ID 5

#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
struct framebuffer {
	unsigned int width;
	unsigned int height;
	sfUint8 *pixels;
};
typedef struct framebuffer framebuffer_t;

struct window {
	sfTexture *texture;
	sfSprite *sprite;
};

typedef struct square {
	int color;
	int life;
} square_t;

typedef struct window window_t;
int epilepsy(sfRenderWindow *window, sfEvent event, window_t *elements, framebuffer_t *pixel);
int noise(sfRenderWindow *window, sfEvent event, window_t *elements, framebuffer_t *pixel);
int triangle(sfRenderWindow *window, sfEvent event, window_t *elements, framebuffer_t *pixel);
int rectangle(sfRenderWindow *window, sfEvent event, window_t *elements, framebuffer_t *pixel);
int square(sfRenderWindow *window, sfEvent event, window_t *elements, framebuffer_t *pixel);
void destroy_all(sfRenderWindow *window, window_t *elements, framebuffer_t *pixel);
void display_framebuffer(framebuffer_t *pixel, window_t *window, sfRenderWindow *display);
void my_putpixel(framebuffer_t *buffer, unsigned int width, unsigned int height, sfColor color);
void draw_rect(framebuffer_t *pixel, int x, int y, int a, int b, sfColor color);
void fade(framebuffer_t *buffer);
#endif //PIXEL_H_
