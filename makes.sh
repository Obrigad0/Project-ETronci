#!/bin/bash

ROOT=$(pwd)
CURRENT_ROOT=""

CLEAN=false

check_arguments() {
    if [ "$1" = "--clean" ] || [ "$1" = "-c" ]
    then
        CLEAN=true
    fi
}

check_last_command() {
    if [ $? -ne 0 ]
    then
        printf "\n\n\n!!!!!!!!!! Sfortunatamente Errore :((((( !!!!!!!!!!\n"
        exit
    fi
}

try_cd() {
    cd $1

    check_last_command
}

set_current_root() {
    CURRENT_ROOT=$1

    try_cd "$ROOT/$CURRENT_ROOT"
}

try_make() {
    if $CLEAN = true
    then
        make clean
    else
        make
    fi

    check_last_command
}

make_folder() {
    try_cd $1

    printf "INSIDE $1\n"

    try_make

    printf "DONE $1\n\n"

    try_cd "$ROOT/$CURRENT_ROOT"
}

check_arguments $1

printf "\n\n\n########## Inizio script  ##########\n\n\n\n"

printf "############## Service ##############\n\n"

set_current_root src/service

make_folder database
make_folder redis

printf "############## Shared ##############\n\n"

set_current_root src

make_folder shared

printf "############## Classes ##############\n\n"

set_current_root src

make_folder classes

printf "############## Server ##############\n\n"

set_current_root src

make_folder server

printf "############## Logdb ##############\n\n"

set_current_root src

make_folder service/logDatabase

printf "############# Non-functional req Monitor #########\n\n"

set_current_root src

run_main req-monitor

printf "############## Customer ##############\n\n"

set_current_root src/customer

make_folder customer-main

make_folder customer-functions/register-customer
make_folder customer-functions/review-order
make_folder customer-functions/search-products
make_folder customer-functions/check-order
make_folder customer-functions/make-order

printf "############## Seller ##############\n\n"

set_current_root src/seller

make_folder seller-main

make_folder seller-functions/add-product
make_folder seller-functions/register-seller
make_folder seller-functions/update-product

printf "############## Courier ##############\n\n"

set_current_root src/courier

make_folder courier-main

make_folder courier-functions/register-courier
make_folder courier-functions/update-delivery-status
make_folder courier-functions/take-delivery
make_folder courier-functions/view-pending-orders

printf "\n\n\n########## Make script completed ###########\n\n\n\n"