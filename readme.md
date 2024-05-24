# Age of Sources

Age of Sources is a real time strategy game spiritually in line with games like Dune 2, Command and Conquer: Red Alert, and Warhammer 40k: Dawn of War II. It depicts a war between the tanuki civilization and U.N.E.C.T.A.S., a dangerous foreign entity which is after their resources.

To fight Unectas (the leader of U.N.E.C.T.A.S.), the player will have to build a town and a varied army of cute tanukis their allies.

## How to play

Use the arrow keys to move on the map, select your units with box selection, move them to your mouse's position with right click. Use a unit's ability when it's selected by left-clicking on the grid in the bottom-right of the screen.
To win, destroy the ennemy's forum.

## Design

### Economy
Each player must pile up resources in the form of clay and water to build their town and army. Production of resources originates from wells and furnaces and is passive.
Buildings can be constructed by beavers, that can be hired from the forum, which is the only building the players originally have.

### Combat
Combat is based on the famous "Rock-Paper-Scissors" system, where each kind of unit is strong against one other type, and weak against one.

### Find out more
More informations on the game's design can be found (in French) on: https://docs.google.com/document/d/1IPbxiTLj0bLq1hyJecttDWtiTuiY5Pz58Ut8i5y4QXE/edit?usp=sharing 
## Technical details

This game's code is heavily documented in the rest of this document. That being said, it still seems relevant to discuss here one of our technical choices:
### On the use of an ECS
An Entity Component System (thereafter reffered to as ECS) is a data structures commonly used in game engines, that stores entities and link them to components. \\
A Component can be any data structure, and an entity is just an abstraction to which is associated a number of components. \\
Not all entities have an instance of each component, and the main benefit of the ECS is that one can get all the entities that have a specific set of components in linear time.

We decided to use an ECS as it is a great tool to structure a game, and as some of us had previous experience implementing them and working with them.


## Compilation

Some dependencies are required to compile, they can be installed with `sudo apt-get install libsdl2-dev libomp-dev libsdl2-ttf-dev libsdl2-mixer-dev` on Debian based distributions.

## Doxygen

The use of `make doc` requires doxygen, LateX and graphviz.

The use of the command `make htmldoc` requires firefox

The html documentation is also available at https://uwu-segfault.eu/2p2doc/. If the site isn't available for some reason (Microsoft Azure), please contact Louis Buisson (louis.buisson@telecomnancy.eu)
