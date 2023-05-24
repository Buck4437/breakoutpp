FLAGS = -std=c++11 -pedantic-errors
NCURSES = -lncurses -lncursesw
MAKE_OBJECT = g++ $(FLAGS) -c $<
MAKE_PROGRAM = g++ $(FLAGS) $^ -o $@ $(NCURSES)

ball.o: src/ball.cpp src/ball.h src/paddle.h src/playing_field.h \
src/ncu.h src/vector2.h src/rect.h src/well.h src/rect_wall.h \
 src/shield.h src/rect_block.h
	$(MAKE_OBJECT)

game_stat.o: src/game_stat.cpp src/game_stat.h src/game_stat_timer.h \
 src/ncu.h src/general_utils.h
	$(MAKE_OBJECT)

game.o: src/game.cpp src/game.h src/ball.h src/paddle.h \
 src/playing_field.h src/ncu.h src/vector2.h src/rect.h src/well.h \
 src/rect_wall.h src/shield.h src/rect_block.h src/game_stat.h \
 src/game_stat_timer.h src/leaderboard.h src/record.h src/level.h \
 src/loot_table.h src/power_up.h src/missile.h src/notification_bar.h \
 src/power_up_drop.h src/power_up_list.h
	$(MAKE_OBJECT)

general_utils.o: src/general_utils.cpp src/general_utils.h src/ncu.h
	$(MAKE_OBJECT)

leaderboard.o: src/leaderboard.cpp src/leaderboard.h src/record.h
	$(MAKE_OBJECT)

level_loader.o: src/level_loader.cpp src/level.h src/ball.h src/paddle.h \
 src/playing_field.h src/ncu.h src/vector2.h src/rect.h src/well.h \
 src/rect_wall.h src/shield.h src/rect_block.h src/game_stat.h \
 src/game_stat_timer.h src/loot_table.h src/power_up.h src/missile.h \
 src/notification_bar.h src/power_up_drop.h src/power_up_list.h
	$(MAKE_OBJECT)

level.o: src/level.cpp src/level.h src/ball.h src/paddle.h \
 src/playing_field.h src/ncu.h src/vector2.h src/rect.h src/well.h \
 src/rect_wall.h src/shield.h src/rect_block.h src/game_stat.h \
 src/game_stat_timer.h src/loot_table.h src/power_up.h src/missile.h \
 src/notification_bar.h src/power_up_drop.h src/math_utils.h src/menu.h \
 src/power_up_list.h
	$(MAKE_OBJECT)

loot_table.o: src/loot_table.cpp src/loot_table.h src/power_up.h \
 src/power_up_list.h
	$(MAKE_OBJECT)

main.o: src/main.cpp src/game.h src/ball.h src/paddle.h \
 src/playing_field.h src/ncu.h src/vector2.h src/rect.h src/well.h \
 src/rect_wall.h src/shield.h src/rect_block.h src/game_stat.h \
 src/game_stat_timer.h src/leaderboard.h src/record.h src/level.h \
 src/loot_table.h src/power_up.h src/missile.h src/notification_bar.h \
 src/power_up_drop.h src/menu.h
	$(MAKE_OBJECT)

math_utils.o: src/math_utils.cpp src/math_utils.h
	$(MAKE_OBJECT)

menu.o: src/menu.cpp src/menu.h src/ncu.h src/leaderboard.h src/record.h
	$(MAKE_OBJECT)

missile.o: src/missile.cpp src/missile.h src/playing_field.h src/ncu.h \
 src/vector2.h src/rect_block.h src/rect_wall.h src/rect.h src/well.h \
 src/shield.h
	$(MAKE_OBJECT)

notification_bar.o: src/notification_bar.cpp src/notification_bar.h \
 src/ncu.h
	$(MAKE_OBJECT)

paddle.o: src/paddle.cpp src/paddle.h src/playing_field.h src/ncu.h \
 src/vector2.h src/rect.h src/well.h src/rect_wall.h src/shield.h \
 src/math_utils.h
	$(MAKE_OBJECT)
 
playing_field.o: src/playing_field.cpp src/playing_field.h src/ncu.h \
 src/vector2.h
	$(MAKE_OBJECT)

power_up_drop.o: src/power_up_drop.cpp src/power_up_drop.h src/paddle.h \
 src/playing_field.h src/ncu.h src/vector2.h src/rect.h src/well.h \
 src/rect_wall.h src/shield.h src/power_up.h
	$(MAKE_OBJECT)

power_up_list.o: src/power_up_list.cpp src/power_up_list.h src/power_up.h
	$(MAKE_OBJECT)

record.o: src/record.cpp src/record.h
	$(MAKE_OBJECT)

rect_block.o: src/rect_block.cpp src/rect_block.h src/rect_wall.h \
 src/playing_field.h src/ncu.h src/vector2.h src/rect.h
	$(MAKE_OBJECT)

rect_wall.o: src/rect_wall.cpp src/rect_wall.h src/playing_field.h \
 src/ncu.h src/vector2.h src/rect.h
	$(MAKE_OBJECT)

rect.o: src/rect.cpp src/rect.h src/vector2.h src/math_utils.h
	$(MAKE_OBJECT)

shield.o: src/shield.cpp src/shield.h
	$(MAKE_OBJECT)

vector2.o: src/vector2.cpp src/vector2.h src/math_utils.h
	$(MAKE_OBJECT)

well.o: src/well.cpp src/well.h src/playing_field.h src/ncu.h \
 src/vector2.h src/rect.h src/rect_wall.h src/shield.h
	$(MAKE_OBJECT)

main: ball.o game_stat.o game.o general_utils.o leaderboard.o level_loader.o \
 level.o loot_table.o main.o math_utils.o menu.o missile.o notification_bar.o \
 paddle.o playing_field.o power_up_drop.o power_up_list.o record.o \
 rect_block.o rect_wall.o rect.o shield.o vector2.o well.o
	$(MAKE_PROGRAM)

clean:
	rm *.o
	rm main

.PHONY: clean