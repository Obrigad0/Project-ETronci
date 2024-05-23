#!/bin/bash

check_last_command() {
    if [ $? -ne 0 ]
    then
        printf "\n\n\n!!!!!!!!!! An error occurred while trying to run the script !!!!!!!!!!\n"
        exit
    fi
}

try_sh() {
    sh $1 $2

    check_last_command
}

try_cd() {
    cd $1

    check_last_command
}

try_cd src
try_cd service
try_cd database
try_sh create.sh
try_cd ..
try_cd ..
try_cd ..
try_sh makes.sh --clean
try_sh makes.sh
