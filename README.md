# m8

<p align="center">
  <img src="https://github.com/mathmoi/m8/blob/master/resources/m8-large.png?raw=true" alt="m8">
</p>

m8 is a chess engine writter in modern C++ by [Mathieu Pagé](https://www.mathieupage.com). m8 is a UCI compatible chess engine that can be used with most chess intefaces.

## How to build

### boost

To build m8 you will need the development version of the boost librairies installed on your system and discoverable by cmake. On a Debian derived Linux distribution you can install them with this command: `sudo apt install libboost-all-dev`. On other systems you will have to install and build boost yourself, then configure [environements variables for cmake](https://cmake.org/cmake/help/latest/module/FindBoost.html#hints) to discover where you boost libraries are.

### build commands

```bash
git clone https://github.com/mathmoi/m8.git
cd m8
mkdir build
cd build
cmake ..
cmake --build . --target m8 -j 32
```

This will produce the m8 executable file (`build/apps/m8/m8`) and a configurations file(`build/apps/m8/m8.json`).

## How to use

### In a chess interface

The usual way to use m8 is to use it as an engine in a chess interface. m8 is tested with [Cute Chess](https://cutechess.com/) and [XBoard](https://www.gnu.org/software/xboard/), but it should work in most interface supporting UCI.

### On the command line

m8 also provide a command line interface with some commands usefull for testing and developpement purposes.

```
usage : m8 [command] [options]

Allowed commands
  uci     Launch m8 in UCI mode (this is the default command).
  analyze Analyze a chess position.
  perft   Run a perft test, counting the nodes reachables from a position at a given depth.
  bench   Run a benchmark.
The command is optional. If a command is not provided, m8 execute in UCI mode.

Allowed options:
  --help                  Produce this help message. Use "m8 <command> --help" 
                          to see the allowed options for a specific command.
  --max-log-severity arg  Define the maximum log severity level (fatal, error, 
                          warning, info, output, input, debug, trace).
  --tt-size arg           Transposition table size in megabytes (must be a 
                          power of two).
```

## Features

- Negamax search with Alpha-Beta Pruning
- Iterative deepening
- Staged move generation
    - Transposition move
    - Captures (ordered by MVV-LVA)
    - Quiet moves
    
## Change log

The change log can be found in the [CHANGELOG.md](CHANGELOG.md) file.

### Versionning

m8 version number use the following format: v[MAJOR].[MINOR].[PATCH] (example: v1.2.3).

- MAJOR is incremented for each releases intended for end-users. For this reasons users will generally used version in the format vX.0 where 'X' is the Releases number.
- MINOR is incremented each time a new features is implemented, an existing features is improved or removed. Version where the MINOR value is not '0' are considered development version and should not be used by end-users.
- PATCH is optional. It is present and incremented when changes are made without significantly modifying the features of the engine.

