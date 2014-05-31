#include "server.h"

///Variables
bool gameover = false;
bool restart_the_game = true;

//Those variables are used by main and the parallel thread
SDL_Rect rcwall_p1;
SDL_Rect rcwall_p2;
SDL_Rect rcwall_p3;
SDL_Rect rcwall_p4;

//It is just in severals function and thats why it is global
//It is is used every time a packet sends
ENetHost *server;

pthread_t Thread_id;


void Broadcast_Packet(char *message, ENetPacket *packet )                                  //The differents flags you may use to send messages
{
    packet = enet_packet_create(message, strlen(message) + 1, ENET_PACKET_FLAG_RELIABLE); //ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT
    enet_host_broadcast (server, 0, packet); //Argument number two are the channel        //ENET_PACKET_FLAG_RELIABLE
}                                                                                         //ENET_PACKET_FLAG_SENT
                                                                                          //ENET_PACKET_FLAG_UNSEQUENCED

int look_for_clients_and_start_the_game(int *players)  //This is function start a thread that compare players position with balls position
{                                                     //and send messages to the client in base of this information
                                                     //this function also receive all the messages from clients and answer to them in base of their requests
    
    ENetPeer *client[5];
    ENetEvent event;
    ENetPacket *packet_player;
    int playes[5]={0,0,0,0,0}; //to controll the connected players
    int player1_input = 0;
    int player2_input = 0;
    int player3_input = 0;
    int player4_input = 0;
    //The array used to send messages
    char return_message[50];
    int New_Client = 0;
    int i;
    gameover = false;
    ///Connection loop
    while(New_Client<4)
    {
        // Look for new clients untill you get 4
        enet_host_service(server, &event, 1000);
        printf("Waiting for clients......\n");
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
                    New_Client++;
                    printf ("A new client connected from Address: %x:%d\n",
                    event.peer -> address.host,event.peer->address.port);

                    for(i=1; i<6; i++)
                    {
                        if(players[i] == 0)
                        {
                            players[i] = event.peer->address.port; //Take a the first free place in the players array
                            //This array could used to retake connection
                            client[i] = event.peer;

                            ///Make a packet to comunnicate to the client who player it gets
                            sprintf(return_message, "player%d", i); //i controll which place in the players array the new player gets
                            packet_player = enet_packet_create(return_message, strlen(return_message) + 1, ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(event.peer, 0, packet_player);
                            printf("Player%d::%d conected...\n\n", i, event.peer->address.port);
                            break;
                        }
                    }
                    break;

            case ENET_EVENT_TYPE_DISCONNECT:
                    //Have to make free a place in players array
                    for(i=1; i<6; i++)
                    {
                        if(players[i] == event.peer->address.port) //Look which client desconnect and make this place free
                        {
                            players[i] = 0;
                            printf("Player%d::%d disconected...\n\n", i, event.peer->address.port);
                            break;
                        }
                    }
                    New_Client --;
                    break;

            case ENET_EVENT_TYPE_RECEIVE:
                    enet_packet_destroy(event.packet);
                    break;

        }
    }

    ///Now there are 4 players so get info om players and the ball
    if (enet_host_service (server, &event, 2000) > 0 &&
       event.type == ENET_EVENT_TYPE_RECEIVE)
    {

        int i;
        sscanf ((char*)event.packet->data, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
            &rcPlayer1.w, &rcPlayer1.h, &rcball.w, &rcball.h, &rcPlayer3.w, &rcPlayer3.h,
            &rcwall_p1.w, &rcwall_p1.h, &rcwall_p2.w, &rcwall_p2.h, &rcwall_p3.w, &rcwall_p3.h,
            &rcwall_p4.w, &rcwall_p4.h);

        rcPlayer2.w = rcPlayer1.w;
        rcPlayer2.h = rcPlayer1.h;

        rcPlayer4.w = rcPlayer3.w;
        rcPlayer4.h = rcPlayer3.h;
    }
    
    ///Create a thread gameloop
    if (pthread_create(&Thread_id, NULL, &ball_move, NULL) !=0)
    {
        perror("pthread_create() error!");
        exit (1);
    }
    else
    {
        printf("\nThread created successfully\n");
    }

    //Start the game
    resetPlayerPosition();
    //Main thread receive packets, decode them and send back info about players
    while(!gameover)
    {
        while(enet_host_service(server, &event, 0))
        {
            switch(event.type)
            {
                case ENET_EVENT_TYPE_RECEIVE:
                    ///DECODE
                    decode2((char*)event.packet->data, &player1_input, &player2_input, &player3_input, &player4_input);
                    ///Broadcast Packet
                    Player_Action(return_message, packet_player, &player1_input, &player2_input, &player3_input, &player4_input);
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    //Have to make free a place in players array
                    for(i=1; i<6; i++)
                    {
                        if(players[i] == event.peer->address.port) //Look which client desconnect and make this place free
                        {
                            players[i] = 0;
                            printf("Player%d::%d disconected...\n\n", i, event.peer->address.port);
                            break;
                        }
                    }
                    New_Client --;
                    if(New_Client == 0){gameover = true;} //If there are no client connect to the client go to the connect loop
                    break;

                case ENET_EVENT_TYPE_CONNECT:
                    printf ("A new client connected from Address: %x:%d\n",
                    event.peer -> address.host,event.peer->address.port);

                    for(i=1; i<6; i++)
                    {
                        if(players[i] == 0)
                        {
                            players[i] = event.peer->address.port; //Take a the first free place in the players array
                            //This array could used to retake connection
                            client[i] = event.peer;

                            ///Make a packet to comunnicate to the client who player it gets
                            sprintf(return_message, "player%d", i); //i controll which place in the players array the new player gets
                            packet_player = enet_packet_create(return_message, strlen(return_message) + 1, ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(event.peer, 0, packet_player);

                            strcpy(return_message, "start_game");
                            packet_player = enet_packet_create(return_message, strlen(return_message) + 1, ENET_PACKET_FLAG_RELIABLE);
                            enet_peer_send(event.peer, 0, packet_player);

                            printf("Player%d::%d conected...\n\n", i, event.peer->address.port);
                            New_Client ++;
                            break;
                        }
                    }
                    break;
            }
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    ENetAddress address;
    int players[] = {0,0,0,0,0};
	
    if (enet_initialize()!=0)
    {
        fprintf(stderr,"An error ocurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    /*Bind the server to the default host. */
    /*A specific host address can be specified by
    enet_address_set_host(& address, "x.x.x.x");*/

    address.host = ENET_HOST_ANY;
    /*Bind the server to port 5950*/
    address.port = 5950;

    server = enet_host_create(& address, ///The address to bind the server host to
                              4,         ///Allow up to 2 clients and/or outgoing connetions
                              2,         ///Allow up to 2 channels to be used, 0 and 1
                              0,         ///Assume any amunt of incoming bandwidth
                              0          ///Assume any amount of utgoing bandwidth
                                );
    if (server == NULL)
    {
       fprintf(stderr, "An error occurred while trying to create an ENet server host. \n");
       exit(EXIT_FAILURE);
    }

    while(restart_the_game)
    {
        look_for_clients_and_start_the_game(players);
        if (pthread_join(Thread_id, NULL) != 0) 
        {
            perror("pthread_join() error\n");
            exit(3);
        }
        else
        {
            printf("pthread_join() successfully\n\n");
        }
    }

    enet_host_destroy(server);
    enet_deinitialize();
    return EXIT_SUCCESS;
}
