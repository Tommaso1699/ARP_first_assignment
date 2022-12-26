# ARP-Hoist-Assignment
Base project structure for the first *Advanced and Robot Programming* (ARP) assignment.
The project provides the basic functionalities for the **Command** and **Inspection processes**, both of which are implemented through the *ncurses library* as simple GUIs. In particular, the repository is organized as follows:
- The `src` folder contains the source code for the Command, Inspection and Master processes.
- The `include` folder contains all the data structures and methods used within the ncurses framework to build the two GUIs. Unless you want to expand the graphical capabilities of the UIs (which requires understanding how ncurses works), you can ignore the content of this folder, as it already provides you with all the necessary functionalities.
- The `bin` folder is where the executable files are expected to be after compilation

## ncurses installation
To install the ncurses library, simply open a terminal and type the following command:
```console
sudo apt-get install libncurses-dev
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
