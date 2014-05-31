#include "window.h"


const int FRAME_PER_SECOND = 20; //30 gick bra
float Intervall;
///Time controll
float NextTick;

void FPS_Fn()
{
    if(NextTick > SDL_GetTicks())
    {
        SDL_Delay(NextTick - SDL_GetTicks());
    }
    NextTick = SDL_GetTicks() + Intervall;
}

void FPS_Init()
{
    NextTick = 0;
    Intervall = 1*1000/FRAME_PER_SECOND;
    return;
}

void *deal_with_input(void* input)
{
     /* Cast the cookie pointer to the right type. */
    event* p = (event*) input;
    int n = 0;
    char thread_message[50];
    ENetPacket *thread_packet;
    strcpy(wall_play, ""); //array to activate the wall the firs time a player come down to one live
    bool keyDown = false;
    SDL_Event event;
    FPS_Init(); //Start klock to controll frames per second to update the screen

    while(!gameover)
    {

        ///Look for events
        while( SDL_PollEvent( &event ))
        {
            ///An event was found
            switch (event.type)
            {
                ///Close button clicked
                case SDL_QUIT:
                gameover = true;
                pthread_exit((void*)42);
                break;

                ///Handle the keybord
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                        case SDLK_q:
                        gameover = true; //Return to main menu and disconnect from the server
                        break;

                        case SDLK_f:
                        if(Mix_PlayChannel(-1,fuck,0 )== -1)
                        {
                            fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
                        }
                        break;

                        case SDLK_LEFT:
                        strcpy(thread_message, p->me);
                        strcat(thread_message, " left");
                        keyDown = true;
                        break;

                        case SDLK_RIGHT:
                        strcpy(thread_message, p->me);
                        strcat(thread_message, " right");
                        keyDown = true;
                        break;

                        case SDLK_UP:
                        strcpy(thread_message, p->me);
                        strcat(thread_message, " up");
                        keyDown = true;
                        break;

                        case SDLK_DOWN:
                        strcpy(thread_message, p->me);
                        strcat(thread_message, " down");
                        keyDown = true;
                        break;
                    }
                    break;

                case SDL_KEYUP:
                strcpy(thread_message, "");
                keyDown = false;
                break;
            }
            if(keyDown)
            {
                thread_packet = enet_packet_create(thread_message, sizeof(thread_message) +1, ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(peer, 0, thread_packet);
                strcpy(thread_message, "");
            }
        }

        if (GOAL) //Print Score on the screen
        {
            n = 20; //35 works god
            GOAL = false;
        }
        else
        {   
            ///Apply the image
            Update_The_Surface(p->me); //pass your player id to this function
            if(n > 0)
            {

                SDL_BlitSurface(scoreMade, NULL, ScreenSurface, &rcscoreMade);
                n--;
            }
            ///Frames per second
            FPS_Fn();
            SDL_UpdateWindowSurface( Window );
        }
    }
    pthread_exit ((void*) 0);
}