gcc -o bin/motor_x src/motor_x.c -Iinclude
gcc -o bin/motor_z src/motor_z.c -Iinclude
gcc -o bin/inspection src/inspection_console.c -Iinclude
gcc -o bin/command src/command_console.c -Iinclude
gcc -o bin/watchdog src/watchdog.c -Iinclude
gcc -o bin/master src/master.c -Iinclude