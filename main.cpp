#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

SDL_Texture* loadTexture( std::string path, SDL_Renderer * render );

// nejake konstanty
static int SCREEN_WIDTH = 1000;
static int SCREEN_HEIGHT = 700;

int main(int argc,char**argv)
{

	SDL_Init(SDL_INIT_VIDEO);

    SDL_Texture * texture;
    SDL_Window * okno;
	okno=SDL_CreateWindow("Earl of Sandwich", //nazev
				SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,  //pozice (posouvatelne okno)
				SCREEN_WIDTH,SCREEN_HEIGHT, //rozmer
				0);	//flags

	SDL_Renderer* render;

	render=SDL_CreateRenderer(okno,-1,SDL_RENDERER_ACCELERATED);

	bool success = true;
    if( render == NULL )
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Initialize renderer color
        SDL_SetRenderDrawColor( render, 0xFF, 0xFF, 0xFF, 0xFF );

        //Initialize PNG loading
        int imgFlags = IMG_INIT_JPG;
        if( !( IMG_Init( imgFlags ) & imgFlags ) )
        {
            printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
            success = false;
        }
    }

    texture = loadTexture("windowsontheworld.jpg", render);

    // vyrob plosinku
	SDL_Rect obdelnik;
	obdelnik.x=450;
	obdelnik.y=650;
	obdelnik.w=100;
	obdelnik.h=20;

    int rychlost = 50;

    // prvni vyrenderovani
    SDL_SetRenderDrawColor(render,255,255,255,255);
    SDL_RenderCopy(render, texture, NULL, NULL);
    SDL_RenderPresent(render);
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
			    if(udalost.key.keysym.sym==SDLK_ESCAPE)
                {
                    // ukonci hru
                    state=0;
                }
                // ziskej stav klaves na klavesnici
                const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
			    // pohyb plosinky doleva a doprava
                if( currentKeyStates[ SDL_SCANCODE_LEFT ] && obdelnik.x > 0)
				{
                    SDL_SetRenderDrawColor(render,0,255,0,255);
                    obdelnik.x = obdelnik.x -rychlost;
				}
                if( currentKeyStates[ SDL_SCANCODE_RIGHT ] && obdelnik.x < SCREEN_WIDTH - obdelnik.w)
				{
                    SDL_SetRenderDrawColor(render,255,100,200,255);
				    obdelnik.x = obdelnik.x +rychlost;
				}
                // prekresli obdelnik
                SDL_RenderCopy(render, texture, NULL, NULL);
                SDL_RenderFillRect(render,&obdelnik);
                SDL_RenderPresent(render);

                SDL_Delay(20);
			}
		}
	}

    // Ukoncit, uklidit
	//Free loaded image
	SDL_DestroyTexture( texture );
	texture = NULL;

	//Destroy window
	SDL_DestroyRenderer( render );
	SDL_DestroyWindow( okno );
	okno = NULL;
	render = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();

return 0;
}

SDL_Texture* loadTexture( std::string path , SDL_Renderer* render )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( render, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}
