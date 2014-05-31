#include "window.h"

///Tevent are the struct that passes to the thread. this is declare in the .h file
event input;
ENetPeer *peer; //Peer to connect to. (Server)
ENetAddress address;
ENetHost *client; // this is your info. That variable is used to look for events
ENetEvent netevent;

bool connect_to_server()
{
        //Initialize enet
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occured while initializing ENet.\n");
        return EXIT_FAILURE;
    }

    atexit(enet_deinitialize);

    ///Create a host using enet_host_create
    client = enet_host_create(NULL,             //Create a client host
                                 1,             //only allow 1 outgoing connection
                                 2,             //allow up 2 channels to be used, 0 and 1
                                 57600/8,       //56K modem with 56 Kbps downstream bandwidth
                                 14400/8);      //56K modem with 14 Kbps upstream bandwidth

    if (client == NULL) {
        fprintf(stderr, "An error occured while trying to create an ENet server host\n");
        exit(EXIT_FAILURE);
    }

    ///Connect to a server with IP address 130.237.84.99 on port 4950//if the server is in a local-network "localhost"
    enet_address_set_host(&address, "130.237.84.99");
    address.port = 5950;

    ///Connect and use service
    peer = enet_host_connect(client, &address, 2, 0);

    if (peer == NULL) {
        fprintf(stderr, "No available peers for initializing an ENet connection");
        exit(EXIT_FAILURE);
    }

    /// Try to connect to server within 5 seconds
   if (enet_host_service (client, &netevent, 5000) > 0 &&
       netevent.type == ENET_EVENT_TYPE_CONNECT)
   {
       printf("Connection to server succeeded.\n\n");

   }

   else
   {
        ///Either the 5 seconds are up or a disconnect netevent was
        ///received. Reset the peer in the netevent the 5 seconds
        ///had run out without any significant netevent.
       enet_peer_reset (peer);

       fprintf (stderr, "Connection to server failed\nServer could be full\n.");
       exit (EXIT_FAILURE);
   }

   if (enet_host_service (client, &netevent, 1000) > 0 &&
       netevent.type == ENET_EVENT_TYPE_RECEIVE)
   {
        if(strcmp((char *)netevent.packet->data, "Server are full!") == 0) //the server cannot take more clients
        {
            printf("%s\n", (char*)netevent.packet->data);
            disconnect_from_server();
            return FAILURE;
        }

        else if(strcmp((char *)netevent.packet->data, "player4") == 0) //The last player to connect send info about players, ball and wall
        {
            strcpy(input.me,(char *)netevent.packet->data); //Save the player id to the thread struct
            //Skicka data om players and ball
            Send_Players_and_Ball_Info();
            return SUCCESS;
        }

        else 
        {
            strcpy(input.me,(char *)netevent.packet->data); ////Save the player id to the thread struct
            return SUCCESS;
        }

   }

}

int disconnect_from_server()
{
    printf("Disconnecting....\n");
    enet_peer_disconnect (peer, 0);
   /// Allow up to 3 seconds for the disconnect to succeed
    /// and drop any packets received packets.
    while (enet_host_service (client, & netevent, 3000) > 0)
    {
        switch (netevent.type)
        {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy (netevent.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                puts ("Disconnection succeeded.....");
                enet_deinitialize();
                ///Free resources and close SDL
                //close();
                return 0;
        }
    }
    ///We've arrived here, so the disconnect attempt didn't
    ///succeed yet. Force the connection down.
    enet_peer_reset (peer);
    ///Free resources and close SDL
	//close();
	enet_deinitialize();
    return 0;
}