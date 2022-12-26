gcc -o bin/motor_x src/motor_x.c -Iinclude
gcc -o bin/motor_z src/motor_z.c -Iinclude
gcc -o bin/inspection src/inspection_console.c -Iinclude -lncurses -lm
gcc -o bin/command src/command_console.c -Iinclude -lncurses
gcc -o bin/master src/master.c -Iinclude
gcc -o bin/select_motor_signals src/Select_motor_signals.c -Iinclude
# gcc -o bin/real_world src/Real_World.c -Iinclude
# gcc -o bin/watchdog src/watchdog.c -Iinclude