#include "server.h"

///Timer
const int FRAME_PER_SECOND = 15;    //Bästa resultat 20
float Intervall;
///Time controll
float NextTick;

//How often the data of the events of the game is send
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

void *ball_move(void *arg)
{
    int counter = 0;
    bool extra_life = true;
    ENetPacket *packet;
    char message[50];
    //Wall info
    int wall_life = 0;
    rcwall_p1.x = 0;
    rcwall_p1.y = SCREEN_HEIGHT-20;
    rcwall_p2.x = 0;
    rcwall_p2.y = 0;
    rcwall_p3.x = 0;
    rcwall_p3.y = 0;
    rcwall_p4.x = SCREEN_WIDTH-20;
    rcwall_p4.y = 0;
    int points[5]={0,9,9,9,9};
    int points_made = 0;

    //Start spelet
    resetPlayerPosition();
    RestartBall(rcball);
    angle = rand() % 361;
    newDirectionBall(angle,rcball);
    sprintf(message, "ball.x %d", rcball.x);
    Broadcast_Packet(message, packet);
    sprintf(message, "ball.y %d", rcball.y);
    Broadcast_Packet(message, packet);
    SDL_Delay(3000);

    strcpy(message, "start_game");
    Broadcast_Packet(message, packet);
    

    FPS_Init();
    //Server gameloop
    while(!gameover)
    {
        
        if(wall_life)
        {
            switch (wall_life)
            {
                case 1: //player 1
                    if(Collition(rcwall_p1, rcball))
                    { ///Collision with bonus wall
                        angle = 360 - angle;
                        rcball.y -= 5;
                        newDirectionBall(angle,rcball);
                        strcpy(message, "wall fin");
                        Broadcast_Packet(message, packet);
                        wall_life = 0;
                    }
                    break;
                case 2: //player 2
                    if(Collition(rcwall_p2, rcball))
                    { ///Collision with bonus wall
                        angle = 360 - angle;
                        rcball.y += 5;
                        newDirectionBall(angle,rcball);
                        strcpy(message, "wall fin");
                        Broadcast_Packet(message, packet);
                        wall_life = 0;
                    }
                    break;
                case 3: //player 3
                    if(Collition(rcwall_p3, rcball))
                    { ///Collision with bonus wall
                        angle = 180 - angle;
                        rcball.x += 5;
                        newDirectionBall(angle,rcball);
                        strcpy(message, "wall fin");
                        Broadcast_Packet(message, packet);
                        wall_life = 0;
                    }
                    break;
                case 4: //player 4
                    if(Collition(rcwall_p4, rcball))
                    { ///Collision with bonus wall
                        angle = 180 - angle;
                        rcball.x -= 5;
                        newDirectionBall(angle,rcball);
                        strcpy(message, "wall fin");
                        Broadcast_Packet(message, packet);
                        wall_life = 0;
                    }
                    break;
            }
        }


        //Look for collision
        if (Collition(rcPlayer1, rcball))/// Returns true if collition is detected //if(rcball.y > rcPlayer1.y)///
        {
            if( (angle > 211 && angle < 329) || (angle > -149 && angle < -31) ){ /// To avoid changing of direction
                angle = 360 - angle - angleEffect(rcball,rcPlayer1,1);
            }
            else{
                angle = 360 - angle;
            }
            rcball.y -= 5;/// to avoid geting stuck and adds a "bounce"-effect
            newDirectionBall(angle,rcball); /// to get new direction on the ball from current location

            strcpy(message, "collision");
            Broadcast_Packet(message, packet);

        }

        if (Collition(rcPlayer2, rcball))/// Returns true if collition is detected ///if(rcball.y < rcPlayer2.y)///
        {
            if( (angle > 31 && angle < 149) || (angle > -329 && angle < -211) ){ /// To avoid non-acceptable direction
                angle = 360 - angle + angleEffect(rcball,rcPlayer2,2);
            }
            else{
                angle = 360 - angle;
            }
            rcball.y += 5;/// to avoid geting stuck and adds a "bounce"-effect
            newDirectionBall(angle,rcball); /// to get new direction on the ball from current location

            strcpy(message, "collision");
            Broadcast_Packet(message, packet);
        }


		if(Collition(rcPlayer3, rcball))/// Returns true if collition is detected
        {
            if( (angle > 121 && angle < 239) || (angle > -239 && angle < -121) ){
                angle = 180 - angle + angleEffect(rcball,rcPlayer3,3);
            }
            else{
                angle = 180 - angle;
            }
            rcball.x += 5;/// to avoid geting stuck and adds a "bounce"-effect
            newDirectionBall(angle,rcball); /// to get new direction on the ball from current location

            strcpy(message, "collision");
            Broadcast_Packet(message, packet);
        }


        if(Collition(rcPlayer4, rcball))/// Returns true if collition is detected
        {
            if( (angle > 301 || angle < 59) || (angle > -59 || angle < -301) )
            {
                angle = 180 - angle - angleEffect(rcball,rcPlayer4,4);
            }
            else
            {
                angle = 180 - angle;
            }
            rcball.x -= 5;/// to avoid geting stuck and adds a "bounce"-effect
            newDirectionBall(angle,rcball); /// to get new direction on the ball from current location

            strcpy(message, "collision");
            Broadcast_Packet(message, packet);
        }


                //Look if it has scored a gol
        ///PLAYER2s WALL
        if(rcball.y < 1){ ///Touched the top of the screen
            points[2]--;
            points_made ++; /// add 1 to points made in the game
            strcpy(message, "score");
            Broadcast_Packet(message, packet);

            sprintf(message, "pmade %d", points_made);
            Broadcast_Packet(message, packet);

            //PLAYERS POINTS
            sprintf(message, "points %d %d %d %d", points[1], points[2], points[3], points[4]);
            Broadcast_Packet(message, packet);

            if (extra_life)
            {
                if( points[2] == 1)
                {
                    strcpy(message, "wall play2");
                    Broadcast_Packet(message, packet); 
                    extra_life = false;
                    wall_life = 2;
                }
            }

            RestartBall(rcball); /// resets ball position and speed
            angle = rand() % 361;/// reset angel to a random one
            newDirectionBall(angle,rcball);/// starts ball in a new direction from center ( bacause we did resetball before)
            points_made ++;/// add 1 to points made in the game
            SDL_Delay(2000);
        }


        ///PLAYER1s WALL
        else if(rcball.y > (SCREEN_HEIGHT - rcball.h - 1) ){ /// touched the bottom of the screen
            points[1]--;
            points_made ++; /// add 1 to points made in the game

            sprintf(message, "pmade %d", points_made);
            Broadcast_Packet(message, packet);

            strcpy(message, "score");
            Broadcast_Packet(message, packet);

            //PLAYERS POINTS
            sprintf(message, "points %d %d %d %d", points[1], points[2], points[3], points[4]);
            Broadcast_Packet(message, packet);

            if (extra_life)
            {
                if( points[1] == 1)
                {
                    strcpy(message, "wall play1");
                    Broadcast_Packet(message, packet); 
                    extra_life = false;
                    wall_life = 1;
                }
            }

            RestartBall(rcball); /// resets ball position and speed
            angle = rand() % 361; /// reset angel to a random one
            newDirectionBall(angle,rcball); /// starts ball in a new direction from center ( bacause we did resetball before)
            SDL_Delay(2000);
        }

        /// PLAYER3s WALL
        else if(rcball.x < 1){
            points[3]--;
            points_made ++;/// add 1 to points made in the game

            sprintf(message, "pmade %d", points_made);
            Broadcast_Packet(message, packet);

            strcpy(message, "score");
            Broadcast_Packet(message, packet);

            //PLAYERS POINTS
            sprintf(message, "points %d %d %d %d", points[1], points[2], points[3], points[4]);
            Broadcast_Packet(message, packet);

            if (extra_life)
            {
                if( points[3] == 1)
                {
                    strcpy(message, "wall play3");
                    Broadcast_Packet(message, packet); 
                    extra_life = false;
                    wall_life = 3;
                }
            }

            RestartBall(rcball);    /// resets ball position and speed
            angle = rand() % 361;/// reset angel to a random one
            newDirectionBall(angle,rcball);/// starts ball in a new direction from center ( bacause we did resetball before)
            SDL_Delay(2000);
        }

        ///PLAYER4s WALL
        else if(rcball.x > (SCREEN_WIDTH - rcball.w -1)){
            points[4]--;
            points_made ++;/// add 1 to points made in the game

            sprintf(message, "pmade %d", points_made);
            Broadcast_Packet(message, packet);

            strcpy(message, "score");
            Broadcast_Packet(message, packet);

            //PLAYERS POINTS
            sprintf(message, "points %d %d %d %d", points[1], points[2], points[3], points[4]);
            Broadcast_Packet(message, packet);

            if (extra_life)
            {
                if( points[4] == 1)
                {
                    strcpy(message, "wall play4");
                    Broadcast_Packet(message, packet); 
                    extra_life = false;
                    wall_life = 4;
                }
            }

            RestartBall(rcball);  /// resets ball position and speed
            angle = rand() % 361;/// reset angel to a random one
            newDirectionBall(angle,rcball);/// starts ball in a new direction from center ( bacause we did resetball before)
            SDL_Delay(2000);
        }


        //Look if some player lost
        if(points[1]==0 || points[2]==0 || points[3]== 0 || points[4]==0)
        {
            strcpy(message, "gameover");
            Broadcast_Packet(message, packet);
            pthread_exit(NULL);
        }


        MoveBall(rcball);
        sprintf(message, "ball.x %d", rcball.x);
        Broadcast_Packet(message, packet);
        sprintf(message, "ball.y %d", rcball.y);
        Broadcast_Packet(message, packet);
        FPS_Fn();
    }

}