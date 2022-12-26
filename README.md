# ARP-Hoist-Assignment
Solution to the first *Advanced and Robot Programming* (ARP) assignment.

Authors:
JT2-Wimir
- Tomasz Maciej Strzesak (5714359)
- Jan Mikołaj Drozd (5646665)

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


## User guide
By pressing Vx+/Vz- button, we increase a velocity of hoist on x/z axis. By pressing Vx-/Vzbutton, we decrease a velocity of hoist on x/z axis. By pressing one of the two stop buttons on command console, we stop hoist movement on x/z axis. On inspection console, if we press Stop button, it stops hoist’s motors, even if they are working at the same time. By pressing Reset button, hoist comes back to its initial position.

## Repository link
```
gh repo clone Tommaso1699/ARP_first_assignment
```
