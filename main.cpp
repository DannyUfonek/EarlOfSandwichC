#include <iostream>
#include <SDL.h>

// nejake konstanty
static int SCREEN_WIDTH = 1000;
static int SCREEN_HEIGHT = 700;

int main(int argc,char**argv)
{

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window * okno;
	okno=SDL_CreateWindow("Earl of Sandwich", //nazev
				SDL_WINDOWPOS_UNDEFINED,           // initial x position
                SDL_WINDOWPOS_UNDEFINED,  //pozice
				SCREEN_WIDTH,SCREEN_HEIGHT, //rozmer
				0);	//flags

	SDL_Renderer* render;

	render=SDL_CreateRenderer(okno,-1,SDL_RENDERER_ACCELERATED);

    // vyrob plosinku
	SDL_Rect obdelnik;
	obdelnik.x=450;
	obdelnik.y=650;
	obdelnik.w=100;
	obdelnik.h=20;

    int rychlost = 50;

    // prvni vyrenderovani
    SDL_SetRenderDrawColor(render,255,255,255,255);
    SDL_RenderClear( render);

    SDL_SetRenderDrawColor(render,0,0,0,255);
    SDL_RenderFillRect(render,&obdelnik);
    SDL_RenderPresent(render);

	int state=1;
	while(state)
	{
		SDL_Event udalost;
		while(SDL_PollEvent(&udalost))
		{
            // chytani klavesnice
			if(udalost.type==SDL_QUIT)
				// ukonci hru
				state=0;

			if(udalost.type==SDL_KEYDOWN)
			{
			    // pohyb plosinky
				if(udalost.key.keysym.sym==SDLK_LEFT && obdelnik.x > 0)
                {
                    obdelnik.x = obdelnik.x -rychlost;
                }
                if(udalost.key.keysym.sym==SDLK_RIGHT && obdelnik.x < SCREEN_WIDTH - obdelnik.w)
                {
                    obdelnik.x = obdelnik.x +rychlost;
                }

                SDL_SetRenderDrawColor(render,255,255,255,255);
                SDL_RenderClear( render);

                SDL_SetRenderDrawColor(render,0,0,0,255);
                SDL_RenderFillRect(render,&obdelnik);
                SDL_RenderPresent(render);

                SDL_Delay(20);
			}
		}
	}

    // Ukoncit, uklidit
	SDL_DestroyWindow(okno);

	SDL_Quit();

return 0;
}
