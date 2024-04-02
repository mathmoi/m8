# m8 changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/)

## [Unreleased](#unreleased)

### Fixed

- Fixed a bug in the way mat scores were reported in plies instead of full move in UCI mode.
- Fixed a couple of bugs in the way the analyze command table was displayed.

## [v0.4](#v-0-4) - 2024-01-01

### Added
- A README and a CHANGELOG file.

### Changed

- Modified the board evaluation to used a tapered evaluation between middle game values and endgame values.
- Modified the piece-square table values to use thoses of PeSTO.

## [v0.3](#v-0-3) - 2024-03-25

### Added

- A command line sub command named analyze that allows to analyse a position on the command line.