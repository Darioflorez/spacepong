#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <enet/enet.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include <SDL2/SDL.h>

#define MAX 100
#define LEFT 1
#define RIGHT 2
#define UP  3
#define DOWN 4
#define SCREEN_WIDTH  900
#define SCREEN_HEIGHT  700

extern bool gameover;

extern double acc_vel;
extern double angle;
extern double scale_x, scale_y;
extern double Resultante;
extern double Vel_x, Vel_y;
///Make the ball move smoothly
extern int StartPosition_x, StartPosition_y;

//Wall extra live
extern SDL_Rect rcwall_p1; //Those variables are used by main and the parallel thread
extern SDL_Rect rcwall_p2;
extern SDL_Rect rcwall_p3;
extern SDL_Rect rcwall_p4;

///Players
extern SDL_Rect rcPlayer1; //Those variables are used by main and the parallel thread
extern SDL_Rect rcPlayer2;
extern SDL_Rect rcPlayer3;
extern SDL_Rect rcPlayer4;
///Ball Parameter
extern SDL_Rect rcball;




///functions
void decode2(char *packet, int (*player1_input), int (*player2_input), int (*player3_input), int (*player4_input)); //Save the inf from the client
void Player_Action(char *return_message, ENetPacket *packet_player, int (*player1_input), int (*player2_input), int (*player3_input), int (*player4_input));//move the players and then send then broadcast the new players position
void MoveBall(struct SDL_Rect &ball);
void Broadcast_Packet(char *message, ENetPacket *packet);
void FPS_Fn();//Controll that it sends data to the client at a certain rate
void FPS_Init();//Start the clock tha controll this rate
void newDirectionBall(double angle, struct SDL_Rect &ball);
void RestartBall(struct SDL_Rect &ball);
void resetPlayerPosition();
double angleEffect(struct SDL_Rect ball, struct SDL_Rect player, int playernum);
double distance( int x1, int y1, int x2, int y2 );
bool Collition(struct SDL_Rect player, struct SDL_Rect ball);
int look_for_clients_and_start_the_game(int *players, ENetHost *server);
void *ball_move(void *arg);//Thread function. Here is where all the logic of the game go on

#endif // SERVER_H_INCLUDED