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

		// pro sprity, ktere nemaji hned jit na obrazovku (hodi se hlavne pro projektily)
        bool hidden;
		//Loads a previously created texture into sprite's texture
		bool loadTexture(SDL_Texture*newTexture, int width = -1, int height = -1);

		//Deallocates texture
		void free();

		//vyrenderuje Sprite podle jeho Rectu
		void render(SDL_Renderer* renderer, SDL_Rect* clip = NULL );

        // moznost - predelat na getX,getY,getW,getH
        SDL_Rect sRect;

        //void update();


        // DULEZITE - SPRITE NEVI, VE KTERE GROUP JE A MUZE BYT POUZE V JEDNE GROUP

        //private:
		//samotna textura
		SDL_Texture* sTexture;

};

class Group
{
    public:
        //class pro rychlou praci s vice Sprity (zatim nepouzite)

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

        //void updateAll();

        void freeAll();

        //void renderAll(SDL_Renderer* renderer);
        //vyprazdni Group, ale nevymaze sprity
        void removeAll();

};

//==============================HERNI TRIDY===============================

enum projectileState{FREE,     // --leti v okne
                     COLLIDING, // --v prubehu srazky s obdelnikem
                     LOST,      // --vyletela z hraciho
                     HIDDEN   // -- zatim nic nedela
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

        bool hasCollided;

        // posune kulicku, animuje , zkontroluje kolizi etc.
        projectileState update(Sprite * plosinka);

};


class glyphProjectile:public Projectile
{
    public:
        //initialize
        glyphProjectile(char character, int velx, int vely, int posx, int posy);
        //deallocate
        ~glyphProjectile();

        char glyph;

};


#endif // SPRITE_H_INCLUDED

