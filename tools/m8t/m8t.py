#!/usr/bin/env python3

import yaml
import chess.pgn
import subprocess
import os
import sys

class Parameters:
    def __init__(self, data):
        self.pgn_file      = data['pgn-file']
        self.concurency    = data['concurency']
        self.openings_file = data['openings_file']
        self.tc            = data['tc']

class Engine:
    def __init__(self, data):
        self.name    = data['name']
        self.command = data['command']

class Gauntlet:
    def __init__(self, data):
        self.engine    = data['engine']
        self.opponents = data['opponents']
        self.rounds    = data['rounds']

class Config:
    def __init__(self, data):
        self.parameters = Parameters(data['parameters'])
        self.engines = {engine['name']: Engine(engine) for engine in data['engines']}
        self.gauntlets = [Gauntlet(gauntlet) for gauntlet in data['gauntlets']]

def ReadConfig(config_filename):
    with open(config_filename, 'r') as fichier:
        data = yaml.safe_load(fichier)
    return Config(data)


def ReadMatchesToPlay(config):
    matches_to_play = {}
    for gauntlet in config.gauntlets:
        for opponent in gauntlet.opponents:
            match_set = frozenset([gauntlet.engine, opponent])
            rounds_to_play = matches_to_play.get(match_set, 0)
            matches_to_play[match_set] = max(rounds_to_play, gauntlet.rounds)
    return matches_to_play

def RemoveGamedPlayed(pgn_filename, matches_to_play):
    if os.path.isfile(pgn_filename):
        with open(pgn_filename) as pgn_file:
            while True:
                game = chess.pgn.read_game(pgn_file)
                if game is None:
                    break
                
                if game.headers['Result'] == '1-0' or game.headers['Result'] == '0-1' or game.headers['Result'] == '1/2-1/2':
                    match_set = frozenset([game.headers["White"], game.headers["Black"]])
                    rounds_to_play = matches_to_play.get(match_set)
                    if rounds_to_play is not None:
                        if rounds_to_play > 1:
                            matches_to_play[match_set] = rounds_to_play - 1;
                        else:
                            matches_to_play.pop(match_set)

def PlayMatches(matches_to_play, config):
    for match_key, rounds in matches_to_play.items():
        engines = list(match_key);
        first_engine_command = config.engines[engines[0]].command
        second_engine_command = config.engines[engines[1]].command
        LaunchCuteChessMatch(config.parameters.concurency,
                             rounds,
                             config.parameters.tc,
                             config.parameters.openings_file,
                             config.parameters.pgn_file,
                             first_engine_command,
                             second_engine_command)

def LaunchCuteChessMatch(concurency, rounds, tc, oppenings, pgn_filename, first_engine, second_engine):
    command = f'cutechess-cli -concurrency {concurency} -rounds {rounds} -openings file={oppenings} format=epd order=random -pgnout {pgn_filename} -repeat -each tc={tc} -engine {first_engine} -engine {second_engine}'
    result = subprocess.run(command, shell=True)

config = ReadConfig(sys.argv[1])
matches_to_play = ReadMatchesToPlay(config)
RemoveGamedPlayed(config.parameters.pgn_file, matches_to_play)
PlayMatches(matches_to_play, config)

