#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <enet/enet.h>
#include <pthread.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

///Screen dimension constants
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 700

extern bool GOAL;
extern bool FAILURE;
extern bool SUCCESS;
extern bool print_gameover;
extern bool start_game;
extern bool gameover;

typedef struct //struc that sends to the thread
{    		
	int counter;
   	char me[20];
}event;
extern event input;

extern ENetPeer *peer; //Peer to connect to. (Server)
extern ENetAddress address;
extern ENetHost *client; // this is your info. That variable is used to look for events
extern ENetAddress address;
extern ENetHost *client;
extern ENetPeer *peer;
extern ENetEvent netevent;

///The window we'll be rendering to
extern SDL_Window *Window;
///The surface contained by the window
extern SDL_Surface *ScreenSurface;

///The image we will load and show on the bakground of the screen
extern SDL_Surface* XOut;
//The menu image
extern SDL_Surface* Menu_Image;
extern SDL_Surface* OPT_Image;

//Score a point
extern SDL_Surface* scoreMade;
extern SDL_Rect rcscoreMade;


///The ball
extern SDL_Surface* Ball;
///Ball position
extern SDL_Rect rcball;

///All Players
extern SDL_Surface* Player1;
extern SDL_Surface* Player2;
extern SDL_Surface* Player3;
extern SDL_Surface* Player4;

extern SDL_Surface* Player1_text;
extern SDL_Surface* Player2_text;
extern SDL_Surface* Player3_text;
extern SDL_Surface* Player4_text;

extern SDL_Rect rcPlayer1;
extern SDL_Rect rcPlayer2;
extern SDL_Rect rcPlayer3;
extern SDL_Rect rcPlayer4;

extern SDL_Rect rcPlayer1_text;
extern SDL_Rect rcPlayer2_text;
extern SDL_Rect rcPlayer3_text;
extern SDL_Rect rcPlayer4_text;

//invisible players
extern SDL_Surface* InvisablePlayer1;
extern SDL_Surface* InvisablePlayer2;
extern SDL_Surface* InvisablePlayer3;
extern SDL_Surface* InvisablePlayer4;

//Wall
extern SDL_Surface* wall_p1;
extern SDL_Rect rcwall_p1;
extern char wall_play[20];
extern bool wall;

extern SDL_Surface* wall_p2;
extern SDL_Rect rcwall_p2;

extern SDL_Surface* wall_p3;
extern SDL_Rect rcwall_p3;

extern SDL_Surface* wall_p4;
extern SDL_Rect rcwall_p4;

//Score
extern SDL_Surface* text1;
extern SDL_Surface* text2;
extern SDL_Surface* text3;
extern SDL_Surface* text4;

extern SDL_Rect rctext1;
extern SDL_Rect rctext2;
extern SDL_Rect rctext3;
extern SDL_Rect rctext4;


extern int points[5];
extern int points_made;
///This variables are used to activate the wall effect
extern int points_made;
extern char wall_play[20];

///Font
extern TTF_Font* font;
extern TTF_Font* font2;
extern TTF_Font* font3;
extern TTF_Font* font4;

//end of game
extern SDL_Rect rcendofgame;
extern SDL_Surface* endofgame;

extern SDL_Color colors[1000];
extern SDL_Color color[2];
extern const char* score[1000];


///The Music
extern Mix_Chunk* effect;
extern Mix_Music* music;
extern Mix_Chunk* music_start;
extern Mix_Chunk* collision;
extern Mix_Chunk* GM_over;
extern Mix_Chunk* fuck;
extern double test;

extern bool create_window(); //Create a window where the game is going to live
extern bool loadMedia(); //Load all the nessesary info for the play to works. Like all the pictures and music
extern int go_to_menu_and_connect_to_the_server(); //Try to connect to the server and then stay here untill the game is over 
extern bool close(); //deallocate all the resources 
extern int Update_The_Surface(char *me); //
extern int mainMenu(SDL_Window*, SDL_Surface*, TTF_Font*, Mix_Chunk*);
extern int nextMenu(SDL_Window*, SDL_Surface*, TTF_Font*, Mix_Chunk*);
extern int instMenu(SDL_Window*, SDL_Surface*, TTF_Font*, Mix_Chunk*);
extern int loadMenu(SDL_Window*, SDL_Surface*, TTF_Font*, Mix_Chunk*);
extern int disconnect_from_server();
extern bool connect_to_server();
extern int game_over_and_restart_the_game(); //restart all the variables
extern void Send_Players_and_Ball_Info(); //The player N 4 uses this function to tell the server the features of objects of the game
extern void FPS_Fn(); //Controll that the screen updates at a certain rate
extern void FPS_Init(); //Start the clock that is going to be used to update the screen
extern void *deal_with_input(void* input); //Thread function. Look at inputs and send it to the server
extern void decode_packet(char* packet); //Decode the packets and save the information that later is used to update the screen
#endif // WINDOW_H_INCLUDED
