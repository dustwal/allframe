GCC=g++
LIBS=`pkg-config --libs allegro-5.0`
FLAGS=-Wall -g -std=c++14 -O0

test_run: game_frame.o test.o
	$(GCC) $(LIBS) $(FLAGS) -o $@ $^

test.o: test.cpp game_frame.o
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

game_frame.o: game_frame.cpp game_frame.h game.inl
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

clean:
	rm -f *.swp *.o *run
