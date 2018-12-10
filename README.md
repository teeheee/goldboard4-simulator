# Goldboard4-Simulator

This is a test to see if it is possible to write a small simulator for the Goldboard.
It is based on simavr because simavr is the only simulator that supports i2c interface (realy?).
The code needs to be written in C, because the Makefiles are quite complicated to adopt (see object_orientated branch).
The codebase written by me is in the goldboard folder. The rest is simavr related stuff.

# Roadmap

## Basic Simulator

* emulate every I2c device we've got
* fetch motor PWM and show motor rotation
* visualize with simple ncurses interface

## Advanced Simulator

* configure Robot with Json file (gui?)
* make a small simulation enviroment with ball and opponents
