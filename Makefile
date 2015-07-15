GCC=g++
LIBS=`pkg-config --libs allegro_primitives-5.0 allegro_font-5.0 allegro_image-5.0 allegro_ttf-5.0`
FLAGS=-Wall -Wextra -Wformat=2 -pedantic -g -O0
STD=-std=c++14

all: mini_run mini_games_run input_test_run pong_run display_tool_run

c11: c_11 all

c_11:
	$(eval STD=-std=c++11)

mini_run: examples/mini_game.o core/game_frame.o core/common.o
	$(GCC) $(STD) $(FLAGS) -o $@ $^ $(LIBS) 

input_test_run: tools/button_tool.o core/game_frame.o core/common.o
	$(GCC) $(STD) $(FLAGS) -o $@ $^ $(LIBS) 

mini_games_run: examples/mini_games.o core/game_frame.o core/common.o
	$(GCC) $(STD) $(FLAGS) -o $@ $^ $(LIBS) 

pong_run: examples/pong_main.o examples/pong.o core/game_frame.o core/common.o
	$(GCC) $(STD) $(FLAGS) -o $@ $^ $(LIBS) 

display_tool_run: tools/display_modes.o core/game_frame.o core/common.o
	$(GCC) $(STD) $(FLAGS) -o $@ $^ $(LIBS) 

examples/mini_game.o: examples/mini_game.cpp core/game_frame.h core/common.h core/game.inl
	$(GCC) $(STD) $(FLAGS) -c $< -o $@ $(LIBS) 

tools/button_tool.o: tools/button_tool.cpp core/game_frame.h core/common.h core/game.inl
	$(GCC) $(STD) $(FLAGS) -c $< -o $@ $(LIBS) 

examples/mini_games.o: examples/mini_games.cpp core/game_frame.h core/common.h core/game.inl
	$(GCC) $(STD) $(FLAGS) -c $< -o $@ $(LIBS) 

examples/pong_main.o: examples/pong_main.cpp examples/pong.h core/game_frame.h core/common.h core/game.inl
	$(GCC) $(STD) $(FLAGS) -c $< -o $@ $(LIBS) 

examples/pong.o: examples/pong.cpp core/game_frame.h core/common.h core/game.inl examples/pong.h
	$(GCC) $(STD) $(FLAGS) -c $< -o $@ $(LIBS) 

tools/display_modes.o: tools/display_modes.cpp core/game_frame.h core/common.h
	$(GCC) $(STD) $(FLAGS) -c $< -o $@ $(LIBS) 

test/test.o: test/test.cpp core/game_frame.o core/game.inl
	$(GCC) $(STD) $(FLAGS) -c $< -o $@ $(LIBS) 

core/common.o: core/common.cpp core/common.h core/game_frame.h
	$(GCC) $(STD) $(FLAGS) -c $< -o $@ $(LIBS) 

core/game_frame.o: core/game_frame.cpp core/game_frame.h 
	$(GCC) $(STD) $(FLAGS) -c $< -o $@ $(LIBS) 

clean:
	rm -f *.swp *.o *run core/*.o examples/*.o test/*.o
