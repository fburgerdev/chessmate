# Description
C++ command line chess-engine

## Features
- gameserver run via command line
    - creategame -> GAMEID
    - joingame GAMEID -> PLAYERID, PLAYER("w" or "b")
    - domove GAMEID PLAYERID MOVE(in UCI Standard)
    - gamestate GAMEID -> GAMESTATE

# System-Support
- linux: supported, tested, prebuild binaries
- windows: supported, tested, build yourself

# Use Application
- executable at "PATH_TO_REPO/bin/CONFIG/example"