#include <stdio.h>

#include "i_video.h"
#include "SDL/SDL.h"

static const int SCREEN_BPP = 16;

static SDL_Surface *screen = NULL;

aFb  *video_init(int width, int height)
{
	static aFb fb;

	printf("SDL_Init...");
	if( SDL_Init( SDL_INIT_VIDEO ) == -1 ) {
printf("cannot init sdl :-(\n");
		return NULL;
	}
	printf("OK\n");
	
	screen = NULL;

	screen = SDL_SetVideoMode( width, height, SCREEN_BPP, SDL_SWSURFACE );
	
	if( screen == NULL ) {
printf("cannot set video mode :-(\n");
		return NULL;
	}
	
	SDL_WM_SetCaption( "aDoom", NULL );

printf("Allocated screen %dx%d (%d) screen %p, screen->pixels %p\n", screen->w, screen->h, screen->pitch, (void*)screen, screen->pixels);

	*(Uint16*)screen->pixels = 0xffff;

	fb.pixels = (Uint16*)screen->pixels;
	fb.pitch  = screen->pitch;

	return &fb;
}

void video_flip(void)
{
	if( SDL_Flip( screen ) == -1 ) {
printf("Can't blit ???\n");
		return;
	}
}

void  video_exit(void)
{
printf("SDL exit\n");
	SDL_Quit();
}

