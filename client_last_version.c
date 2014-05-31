#include "window.h"

//The flags to controll what is going on the game
bool gameover = false;
bool FAILURE = false;
bool SUCCESS = true;
bool GOAL = false;
bool wall = false;
bool print_gameover = false;
bool play_the_game = true;
bool start_game = false;

//To created the thread
pthread_t Thread_id;

int main(int argc, char **argv)
{
    ///Start up SDL and create window
    int controll = 0;
    int JoinThread = 0;
	if( !create_window())
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		///Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
            while(play_the_game)
            {   
                //All the game plays here
                controll = go_to_menu_and_connect_to_the_server(); 

                if ( controll < 0)
                {
                    play_the_game = false;
                    disconnect_from_server();
                    ///Free resources and close SDL
                    close();
                    return EXIT_FAILURE;
                }
                else if (controll == 2) //User presed QUIT or EXIT and it was not connected to the server
                {
                    play_the_game = false;
                    ///Free resources and close SDL
                    if(close())
                    {
                        return EXIT_SUCCESS;
                    }
                    else 
                    {
                        return EXIT_FAILURE;
                    }

                }

                if (pthread_join(Thread_id,(void **) &JoinThread) != 0) 
                {
                    perror("pthread_join() error");
                    exit(3);
                }
                else
                {
                    printf("pthread_join() successfully\n\n");
                }

                if(JoinThread != 0) //The user presed QUIT during the game.
                {
                    disconnect_from_server();
                    ///Free resources and close SDL
                    if (close())
                    {
                        return EXIT_SUCCESS;
                    }
                    else
                    {
                        return EXIT_FAILURE;
                    }
                }

                //Play sound game over
                if(Mix_PlayChannel(-1,GM_over,0 )== -1)
                {
                    fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
                }

                game_over_and_restart_the_game();
                disconnect_from_server();
            }
        }
    }
    ///Free resources and close SDL
    close();
    return EXIT_SUCCESS;
}




int go_to_menu_and_connect_to_the_server()
{
    ENetPacket *packet;//Save the data from the server
    gameover = false;
    int controll=0;
    //Load menu and connect to the server
    controll = loadMenu(Window, ScreenSurface, font, effect);
    if(controll < 0)
    {
        printf("Failed to load menu!\n");
        return -1;
    }
    else if(controll==2)
    {
        return 2; //Close the window
    }

    ///Create thread
    if (pthread_create(&Thread_id, NULL, &deal_with_input, &input) != 0) 
    {
        perror("pthread_create() error");
        exit(1);
    }
    else
    {
        printf("\nThread created successfully\n");
    }

    if(Mix_PlayChannel(-1,music_start,0 )== -1)
    {
        fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
    }

    //FPS_Init();
    while(!gameover)
    {
        // If we had some netevent that interested us
        while(enet_host_service(client, &netevent, 0))
        {
            switch(netevent.type)
            {
                case ENET_EVENT_TYPE_RECEIVE:
                    printf("(Client) Message from server : %s\n", netevent.packet->data);
                    decode_packet((char *)netevent.packet->data);
                    enet_packet_destroy(netevent.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("(Client) %s Server disconnected.....\n\nGAME OVER!!!", (char* )netevent.peer->data);
                    gameover = true;
                    start_game = false;
                    /// Reset client's information
                    netevent.peer->data = NULL;
                    return -1;
                    break;

                case ENET_EVENT_TYPE_NONE:
                    break;
            }
        //SDL_Delay(1000/60); //hämta 60 packet varje sekund
        //Frames per second
        //FPS_Fn();
        ///Apply the image
        //Update_The_Surface();
        }
    }
    return 0;
}

void Send_Players_and_Ball_Info()
{
    char message[200];
    ENetPacket *packet;

    sprintf(message,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
        rcPlayer1.w, rcPlayer1.h, rcball.w, rcball.h, rcPlayer3.w, rcPlayer3.h,
        rcwall_p1.w, rcwall_p1.h, rcwall_p2.w, rcwall_p2.h, rcwall_p3.w, rcwall_p3.h,
        rcwall_p4.w, rcwall_p4.h);
    packet = enet_packet_create(message, sizeof(message) +1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    printf(">>>%s\n!!",message);
}

void decode_packet(char* packet)
{
    char tmp[20];
    int i;
   if(strstr(packet, "player1"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[8]);
        rcPlayer1.x = atoi(tmp);
        printf("Player1: %d\n\n", rcPlayer1.x);
   }

   else if(strstr(packet, "player2"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[8]);
        rcPlayer2.x = atoi(tmp);
        printf("Player2: %d\n\n", rcPlayer2.x);
   }

   else if(strstr(packet, "player3"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[8]);
        rcPlayer3.y = atoi(tmp);
        printf("Player3: %d\n\n", rcPlayer3.y);
   }

    else if(strstr(packet, "player4"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[8]);
        rcPlayer4.y = atoi(tmp);
        printf("Player4: %d\n\n", rcPlayer4.y);
   }

   else if(strstr(packet, "ball.x"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[7]);
        rcball.x = atoi(tmp);
        printf("Ball.x: %d\n\n", rcball.x);
   }

   else if(strstr(packet, "ball.y"))
   {    ///Jag vill att player försvinner
        strcpy(tmp, &packet[7]);
        rcball.y = atoi(tmp);
        printf("Ball.y: %d\n\n", rcball.y);
   }
   else if (strstr(packet, "points"))
   {
        sscanf (packet, "%s %d %d %d %d", 
            tmp, &points[1], &points[2], &points[3], &points[4]);

        text1 = TTF_RenderText_Blended(font3, score[points[1]], colors[points[1]]);
        text2 = TTF_RenderText_Blended(font3, score[points[2]], colors[points[2]]);
        text3 = TTF_RenderText_Blended_Wrapped(font3, score[points[3]], colors[points[3]],30);
        text4 = TTF_RenderText_Blended_Wrapped(font3, score[points[4]], colors[points[4]],30);
   }
   //When someone score a goal SCORE comes up on he screen
   else if (strstr(packet, "score"))
   {

        GOAL = true; 
   }
   //This is used to count how many points have been scored along the game
   //Every five points scored all the players become invisible
   else if (strstr(packet, "pmade"))
   {
       sscanf (packet, "%s %d", tmp, &points_made);
   }

   //The first player to reach 1 point get a extra live in form of a wall
   else if(strstr(packet, "wall"))
   {
        if(strstr (packet, "play"))
        {
            strcpy(wall_play, packet);
            wall = true;
        }
        else if (strstr(packet, "fin"))
        {
            wall = false;
            strcpy(wall_play, "");
        }
   }
   else if(strstr(packet, "gameover"))
   {
        gameover = true;
   }

   else if(strstr(packet, "collision"))
   {
        //Play sound
        if(Mix_PlayChannel(-1,collision,0 )== -1)
        {
            fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
        }
   }
   else if (strstr(packet, "start_game"))
   {
       start_game = true;
   }
}


