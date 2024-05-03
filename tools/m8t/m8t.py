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

def ReadGamesPlayed(pgn_filename):
    games_played = {}
    if os.path.isfile(pgn_filename):
        with open(pgn_filename) as pgn_file:
            while True:
                headers = chess.pgn.read_headers(pgn_file)
                if headers is None:
                    break
                
                if 'Result' in headers and (headers['Result'] == '1-0' or headers['Result'] == '0-1' or headers['Result'] == '1/2-1/2'):
                    key = frozenset([headers["White"], headers["Black"]])
                    games_played[key] = games_played.get(key, 0) + 1
    return games_played
        
def PlayGauntlet(gauntlet, config):
    while True:
        games_played = ReadGamesPlayed(config.parameters.pgn_file)
        games_to_play = []
        for opponent in gauntlet.opponents:
            key = frozenset([gauntlet.engine, opponent])
            games_to_play_this_opponent = gauntlet.rounds - games_played.get(key, 0)
            if games_to_play_this_opponent > 0:
                games_to_play.append((opponent, games_to_play_this_opponent))

        if games_to_play:
            min_games_to_play = min(map(lambda x: x[1], games_to_play))
            engines = [config.engines.get(gauntlet.engine).command] + [config.engines.get(x[0]).command for x in games_to_play]
            LaunchCuteChessGauntlet(config.parameters.concurency,
                                    min_games_to_play,
                                    config.parameters.tc,
                                    config.parameters.openings_file,
                                    config.parameters.pgn_file,
                                    engines)
            games_to_play = [(x[0], x[1] - min_games_to_play) for x in games_to_play if x[1] > min_games_to_play]
            any_games_played = True
        else:
            return

def PlayGauntlets(config):
    for gauntlet in config.gauntlets:
        PlayGauntlet(gauntlet, config)

def LaunchCuteChessGauntlet(concurency, rounds, tc, oppenings, pgn_filename, engines):
    engines_args = ''
    for engine in engines:
        engines_args =  f'{engines_args} -engine {engine}'

    command = f'cutechess-cli -concurrency {concurency} -tournament gauntlet -rounds {rounds} -draw movenumber=60 movecount=10 score=10 -resign movecount=10 score=1500 twosided=true -openings file={oppenings} format=epd order=random -pgnout {pgn_filename} -repeat -recover -each tc={tc} {engines_args} 2>/dev/null'
    print(f'Running: {command}')
    result = subprocess.run(command, shell=True)

try:
    config = ReadConfig(sys.argv[1])
    PlayGauntlets(config)
except KeyboardInterrupt:
    pass
