
C++ = g++
CFLAGS = -lSDL2 -lenet -lpthread
FILES = server_last_version.c server_engine.c game_logik.c handle_players_input.c
OUT_EXE = SERVER

#DEPS = window.h
#OBJ = server_last_version.c server_engine.c
#%.o: %.c $(DEPS)
#       $(CC) -c -o $@ $< $(CFLAGS)
#SERVER: $(OBJ)
#       g++ -o $@ $^ $(CFLAGS)

build: $(FILES)
	$(C++) -o $(OUT_EXE) $(FILES) $(CFLAGS)
clean:
	 rm -f *.o core

rebuild: clean build

