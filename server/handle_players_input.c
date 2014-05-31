#include "server.h"

void decode2(char *packet, int (*player1_input), int (*player2_input), int (*player3_input), int (*player4_input))
{
    char tmp[20];
    if(strstr(packet, "player1 left"))
    {
        *player1_input = LEFT;
    }
    else if(strstr(packet, "player1 right"))
    {
        *player1_input = RIGHT;
    }

    else if(strstr(packet, "player2 left"))
    {
        *player2_input = LEFT;
    }
    else if(strstr(packet, "player2 right"))
    {
        *player2_input = RIGHT;
    }

    else if(strstr(packet, "player3 up"))
    {
        *player3_input = UP;
    }
    else if(strstr(packet, "player3 down"))
    {
        *player3_input = DOWN;
    }

    else if(strstr(packet, "player4 up"))
    {
        *player4_input = UP;
    }
    else if(strstr(packet, "player4 down"))
    {
        *player4_input = DOWN;
    }
}


void Player_Action(char *return_message, ENetPacket *packet_player, int (*player1_input), int (*player2_input), int (*player3_input), int (*player4_input))
{
    switch(*player1_input)
    {
        case LEFT:
            rcPlayer1.x -= 35;
            if(rcPlayer1.x < 0 )
            {
                rcPlayer1.x = 0;
            }
            sprintf(return_message, "player1 %d", rcPlayer1.x);
            Broadcast_Packet(return_message, packet_player);
            *player1_input=0;
            break;

        case RIGHT:
            rcPlayer1.x += 35;
            if(rcPlayer1.x > SCREEN_WIDTH - 200)
            {
                rcPlayer1.x = SCREEN_WIDTH - 200;
            }
            sprintf(return_message, "player1 %d", rcPlayer1.x);
            Broadcast_Packet(return_message, packet_player);
            *player1_input=0;
            break;
    }

    switch(*player2_input)
    {
        case LEFT:
            rcPlayer2.x -= 30;
            if(rcPlayer2.x < 0 )
            {
                rcPlayer2.x = 0;
            }
            sprintf(return_message, "player2 %d", rcPlayer2.x);
            Broadcast_Packet(return_message, packet_player);
            player2_input = 0;
            break;

            case RIGHT:
                rcPlayer2.x += 30;
                if(rcPlayer2.x > SCREEN_WIDTH - 200)
                {
                    rcPlayer2.x = SCREEN_WIDTH - 200;
                }
                sprintf(return_message, "player2 %d", rcPlayer2.x);
                Broadcast_Packet(return_message, packet_player);
                *player2_input = 0;
                break;
    }

    switch(*player3_input)
    {
        case UP:
            rcPlayer3.y -= 30; //20 works good
            if(rcPlayer3.y < 0 )
            {
                rcPlayer3.y = 0;
            }
            sprintf(return_message, "player3 %d", rcPlayer3.y);
            Broadcast_Packet(return_message, packet_player);
            *player3_input = 0;
            break;

        case DOWN:
            rcPlayer3.y += 30;
            if(rcPlayer3.y > SCREEN_HEIGHT - 200)
            {
                rcPlayer3.y = SCREEN_HEIGHT - 200;
            }
            sprintf(return_message, "player3 %d", rcPlayer3.y);
            Broadcast_Packet(return_message, packet_player);
            *player3_input = 0;
            break;
    }

    switch(*player4_input)
    {
        case UP:
            rcPlayer4.y -= 30;
            if(rcPlayer4.y < 0 )
            {
                rcPlayer4.y = 0;
            }
            sprintf(return_message, "player4 %d", rcPlayer4.y);
            Broadcast_Packet(return_message, packet_player);
            *player4_input = 0;
            break;

        case DOWN:
            rcPlayer4.y += 30;
            if(rcPlayer4.y > SCREEN_HEIGHT - 200)
            {
                rcPlayer4.y = SCREEN_HEIGHT - 200;
            }
            sprintf(return_message, "player4 %d", rcPlayer4.y);
            Broadcast_Packet(return_message, packet_player);
            *player4_input = 0;
            break;
    }

}