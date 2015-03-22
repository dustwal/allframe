GCC=g++
LIBS=`pkg-config --libs allegro_primitives-5.0`
FLAGS=-Wall -g -std=c++14 -O0

all: test_run mini_run

mini_run: examples/mini_game.o core/game_frame.o core/common.o
	$(GCC) $(LIBS) $(FLAGS) -o $@ $^

examples/mini_game.o: examples/mini_game.cpp core/game_frame.h core/common.h
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

test_run: core/game_frame.o test/test.o core/common.o
	$(GCC) $(LIBS) $(FLAGS) -o $@ $^

test/test.o: test/test.cpp core/game_frame.o
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

core/common.o: core/common.cpp core/common.h core/game_frame.h
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

core/game_frame.o: core/game_frame.cpp core/game_frame.h core/game.inl
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

clean:
	rm -f *.swp *.o *run core/*.o examples/*.o
