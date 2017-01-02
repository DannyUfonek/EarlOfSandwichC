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

		//Loads a previously created texture into sprite's texture
		bool loadTexture(SDL_Surface * surface, SDL_Renderer* renderer, int customWidth = -1, int customHeight = -1);

		//Deallocates texture
		void free();

		//vyrenderuje Sprite podle jeho Rectu
		void render(SDL_Renderer* renderer, SDL_Rect* clip = NULL );

        // moznost - predelat na getX,getY,getW,getH
        SDL_Rect sRect;


        // DULEZITE - SPRITE NEVI, VE KTERE GROUP JE A MUZE BYT POUZE V JEDNE GROUP

	private:
		//samotna textura
		SDL_Texture* sTexture;

};

class Group
{
    public:
        //class pro rychlou praci s vice Sprity

        //Initializes variables
        Group();

        //Deallocates memory
        ~Group();

        // samotne ukazatele na sprity
        Sprite * members[];

        int numberOfMembers;
        // prida Sprite do skupiny
        void add(Sprite*sprite);
        // odebere Sprite ze skupiny
        void removeFromGroup(Sprite*sprite);

        void freeAll();

        void renderAll(SDL_Renderer* renderer);
        //vyprazdni Group, ale nevymaze sprity
        void removeAll();

};

//==============================HERNI TRIDY===============================

enum projectileState{FREE,     // --leti v okne
                     COLLIDED, // --srazi se s obdelnikem
                     LOST      // --vyletela z hraciho
};

class Projectile:public Sprite
{
    public:
        //Initializes variables
        Projectile(int velx, int vely, int posx, int posy);

        //Deallocates memory
        ~Projectile();

        int velX;
        int velY;
        // posune kulicku, animuje , zkontroluje kolizi etc.
        projectileState update(Sprite * plosinka);
};



#endif // SPRITE_H_INCLUDED

