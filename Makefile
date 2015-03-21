GCC=g++
LIBS=`pkg-config --libs allegro_primitives-5.0`
FLAGS=-Wall -g -std=c++14 -O0

all: test_run mini_run

mini_run: mini_game.o game_frame.o common.o
	$(GCC) $(LIBS) $(FLAGS) -o $@ $^

mini_game.o: mini_game.cpp game_frame.h common.h
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

test_run: game_frame.o test.o common.o
	$(GCC) $(LIBS) $(FLAGS) -o $@ $^

test.o: test.cpp game_frame.o
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

common.o: common.cpp common.h game_frame.h
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

game_frame.o: game_frame.cpp game_frame.h game.inl
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

clean:
	rm -f *.swp *.o *run
