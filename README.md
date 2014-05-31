spacepong
=========
Client:
Just build the make file existing in this project, execute the program with 
./CLIENT and enjoy. It's four players!

Server:
Open the map server, build the make file inside this, execute the server with 
./SERVER and enjoy.
If you want to configure the server's ip-address. You have to go to line 33 
and here you can choose the ip-address you want the server to run from.
If you desire to run the server in a local network please write "localhost"
instead of anyother ip-address.

Requirements:
	*Server:
	A Linux OS distribuitor

	ENet library. You may download this from here http://enet.bespin.org/Downloads.html

	SDL2 library. You may follow this steeps to make it works.
		<http://nothingtocode.blogspot.se/2013/07/setting-up-sdl2-in-ubuntu-or-linux-mint.html>
	

	*Client:
	A Linux OS distribuitor

	ENet library. You may download this from here http://enet.bespin.org/Downloads.html

	SDL2 library. You may follow this steeps to make it works.
		<http://nothingtocode.blogspot.se/2013/07/setting-up-sdl2-in-ubuntu-or-linux-mint.html>

	SDL2_mixer library. You make install it like this
		sudo apt-get install libsdl-mixer-2.0-0 libsdl-mixer-dbg libsdl-mixer-dev

	SDL_ttf library. You may istall it like this
		sudo apt-get install libsdl-ttf2.0-0 libsdl-ttf2.0-0-dev