GCC=g++
LIBS=`pkg-config --libs allegro-5.0`
FLAGS=-Wall -g -std=c++14 -O0

game_engine.o: game_engine.cpp game_engine.hh
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

clean:
	rm *.swp *.o run
