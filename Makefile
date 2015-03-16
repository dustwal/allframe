GCC=g++
LIBS=`pkg-config --libs allegro-5.0`
FLAGS=-Wall -g -std=c++14 -O0

test_run: game_engine.o test.o
	$(GCC) $(LIBS) $(FLAGS) -o $@ $^

test.o: test.cpp game_engine.o
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

game_engine.o: game_engine.cpp game_engine.hh game.inl
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

clean:
	rm -f *.swp *.o *run
