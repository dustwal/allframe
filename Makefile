GCC=g++
LIBS=`pkg-config --libs allegro_primitives-5.0`
FLAGS=-Wall -g -std=c++14 -O0

all: mini_run mini_games_run input_test_run

mini_run: examples/mini_game.o core/game_frame.o core/common.o
	$(GCC) $(LIBS) $(FLAGS) -o $@ $^

input_test_run: tools/button_tool.o core/game_frame.o core/common.o
	$(GCC) $(LIBS) $(FLAGS) -o $@ $^

mini_games_run: examples/mini_games.o core/game_frame.o core/common.o
	$(GCC) $(LIBS) $(FLAGS) -o $@ $^

examples/mini_game.o: examples/mini_game.cpp core/game_frame.h core/common.h core/game.inl
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

tools/button_tool.o: tools/button_tool.cpp core/game_frame.h core/common.h core/game.inl
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

examples/mini_games.o: examples/mini_games.cpp core/game_frame.h core/common.h core/game.inl
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

test/test.o: test/test.cpp core/game_frame.o core/game.inl
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

core/common.o: core/common.cpp core/common.h core/game_frame.h
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

core/game_frame.o: core/game_frame.cpp core/game_frame.h 
	$(GCC) $(LIBS) $(FLAGS) -c $< -o $@

clean:
	rm -f *.swp *.o *run core/*.o examples/*.o test/*.o
