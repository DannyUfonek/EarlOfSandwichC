#include <iostream>
#include <SDL.h>

int main(int argc,char**argv)
{

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window * okno;
	okno=SDL_CreateWindow("GTA VI", //nazev
				10,10,  //pozice
				640,480, //rozmer
				0);	//flags

	SDL_Renderer* render;

	render=SDL_CreateRenderer(okno,-1,SDL_RENDERER_ACCELERATED);

	SDL_Rect obdelnik;
	obdelnik.x=50;
	obdelnik.y=50;
	obdelnik.w=20;
	obdelnik.h=20;


	int i=0;
	for(i=0;i<200;i++)
	{

		obdelnik.x=50+i;

		SDL_SetRenderDrawColor(render,255,255,0,255);
		SDL_RenderClear( render);

		SDL_SetRenderDrawColor(render,0,0,255,255);
		SDL_RenderFillRect(render,&obdelnik);
		SDL_RenderPresent(render);

		SDL_Delay(20);
	}



	SDL_DestroyWindow(okno);

	SDL_Quit();

return 0;
}
