#!/bin/bash

# packages
sudo apt-get install make
sudo apt-get install g++
sudo apt-get install postgresql
sudo apt-get install redis
sudo apt-get install libhiredis-dev
sudo apt install libpq-dev

# commands
sudo usermod -aG $USER postgres
