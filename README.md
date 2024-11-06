# Chessmate
Chessmate is a chess engine written in C++.

It implements the rules of chess and is able to suggest a move using a minimax algoritm.

The move suggestion works as follows:
- calculate all legal moves of the current board state
- for each move, perform it, and repeat these two steps until fixed depth
- then, evaluate the current board position by
    - weighting the pieces (pawn=100, knight=300, bishop=325, rook=500, queen=900)
    - adding weights for each piece, depending on its position using a table
- calculate the board position with the maximum score and suggest the first move leading to this position
> We cut out a lot of legal moves by using the [_alpha-beta pruning_](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning) algorithm 
## Server
You can easily integrate this chess engine in your front end using the server that comes with _chessmate_.
![image](https://github.com/user-attachments/assets/d86d5c54-7300-4984-bd28-1e681ccdcc6b)
### Idea
Each game is associated with a gameID and its state is stored in a json file

Each player is associated with and playerID to prevent cheating (moves can only be performed provided the playerID)
### Usage
#### Create Game
To create a chess game, type the following command:
```console
path/to/server creategame
```
it responds with something like this:
```json
{"gameID":6737,"success":true}
```

#### Join Game
In order to play the game you've created, players must manually join the game.

This is necessary in order to send `playerID`'s to the players.
```console
path/to/server joingame GAME_ID
```
to join with any free color or 
```console
path/to/server -pwhite joingame GAME_ID
```
```console
path/to/server -pblack joingame GAME_ID
```
to add a player for white or black.

The server responds with
```json
{"player":"white","playerID":4852,"success":true}
```
if successfull or
```json
{"error":"game doesn't exist","success":false}
```
Here's a list of errors that can occur:
- game doesn't exist
    - the provided GAME_ID is not associated with an existing game
- game is full
    - both white and black players already joined the game
- player already joined
    - the specified player (white or black) already joined the game

#### Game State
In order to get the current game state, type:
```console
path/to/server gamestate GAME_ID
```
The server responds with something like this:
```json
{
    "gameState": {
        "FEN": "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "active":"w",
        "castle": ["K","Q","k","q"],
        "halfMoves": 1,
        "inCheck": false,
        "legalMoves": [
            "a2a3", "a2a4", "b2b3", "b2b4", "c2c3", "c2c4", "d2d3", "d2d4", "e2e3", "e2e4",
            "f2f3", "f2f4", "g2g3", "g2g4", "h2h3", "h2h4", "b1c3", "b1a3", "g1h3", "g1f3"
        ],
        "history": [],
        "status": "prepare"
    },
    "success": true
}
```
This is how the response can be interpreted:
- `FEN`: the [FEN](https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation) code of the current game state
- `active`: active player ("w" for white and "b" for black)
- `castle`: a list of available castles ("K" white kingside, "Q" white queenside, "k" black kingside, "q" black queenside)
- `halfMoves`: count of halfmoves already played
- `inCheck`: true if the active player is in check
- `legalMoves`: a complete list legal moves the active player can do (following the [UCI Standard](https://en.wikipedia.org/wiki/Universal_Chess_Interface))
- `history`: a history of all moves already played in chronological order ([UCI Standard](https://en.wikipedia.org/wiki/Universal_Chess_Interface))
- `status`:
    - "prepare" if at least one player hasn't joined
    - "inGame" if all players joined and the game is not over
    - "checkmate" if a player won by checkmate
    - "draw" if the game ended with a draw

#### Do Move
To do a move, type the following:
```
path/to/server domove GAME_ID PLAYER_ID MOVE
```
where `MOVE` follows the [UCI Standard](https://en.wikipedia.org/wiki/Universal_Chess_Interface):
- specify start and end square (e.g. e2e4)
- for castling, use the kings origin and destination square (e.g. e1g1 for white kingside)
- for promoting, append the piece character (e.g. e7e8q)

The server responds with something like this
```json
{"success": true}
```
if successfull or
```json
{"error":"game not full","success":false}
```
- game not full (at least one player has not joined the game yet)
- wrong playerid provided (the playerID provided doesn't match with the <u>active</u> player)
- invalid move (the move is either in an invalid format or illegal)
- game over (the game already ended, either with checkmate or draw)

## Installation
```console
git clone https://github.com/fburgerdev/chessmate.git
```

You can locate prebuild binaries (debian) in _bin/dist_, or compile yourself
#### With Make
```console
cd build && make config=dist -j8
```
#### Or with Visual Studio
>open _build/engine.sln_ and compile in _dist_ config