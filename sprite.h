#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

class Sprite
{
	public:
		//Initializes variables
		Sprite();

		//Deallocates memory
		~Sprite();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Deallocates texture
		void freeTexture();

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

#endif // SPRITE_H_INCLUDED
