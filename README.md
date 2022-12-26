# ARP-Hoist-Assignment
Solution to the first *Advanced and Robot Programming* (ARP) assignment.

Authors:
JT2-Wimir
- Tomasz Maciej Strzesak (5714359)
- Jan Mikołaj Drozd (5646665)

- The `src` folder contains the source code for the Command, Inspection and Master processes.
- The `include` folder contains all the data structures and methods used within the ncurses framework to build the two GUIs. Unless you want to expand the graphical capabilities of the UIs (which requires understanding how ncurses works), you can ignore the content of this folder, as it already provides you with all the necessary functionalities.
- The `bin` folder is where the executable files are expected to be after compilation

## Prerequisites
konsole
libncurses

To install the ncurses library, simply open a terminal and type the following command:
```console
sudo apt-get install libncurses-dev
sudo apt install konsole
```

## Compiling and running the code
To compile the program:
```console
./compile.sh
```

To run the program:
```console
./run.sh
```

or

```console
./bin/master
```

## Troubleshooting

Should you experience some weird behavior after launching the application (buttons not spawning inside the GUI or graphical assets misaligned) simply try to resize the terminal window, it should solve the bug.