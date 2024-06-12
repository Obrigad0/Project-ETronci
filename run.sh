#!/bin/bash

ROOT=$(pwd)
CURRENT_ROOT=""

check_arguments() {
    if [ "$1" = "--kill" ] || [ "$1" = "-k" ]
    then
        pkill main
        exit
    fi
}

check_last_command() {
    if [ $? -ne 0 ]
    then
        printf "\n\n\n!!!!!!!!!! An error occurred while trying to run the script !!!!!!!!!!\n"
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

try_main() {
    ./main &

    check_last_command
}

run_main() {
    try_cd $1

    printf "INSIDE $1\n"

    try_main

    printf "DONE $1\n\n"

    try_cd "$ROOT/$CURRENT_ROOT"
}

check_arguments $1

printf "\n\n\n########## Script partito! ##########\n\n\n\n"

pkill main

printf "############## Log Database... ##############\n\n"

set_current_root src

run_main service/logDatabase/bin

printf "\n\n########### Non-functional req Monitor #########\n\n"

set_current_root src

run_main req-monitor/bin

printf "############## Customer... ##############\n\n"

set_current_root src/customer

run_main customer-main/bin

run_main customer-functions/check-order/bin
run_main customer-functions/make-order/bin
run_main customer-functions/register-customer/bin
run_main customer-functions/review-order/bin
run_main customer-functions/search-products/bin


printf "############## Seller... ##############\n\n"

set_current_root src/seller

run_main seller-main/bin

run_main seller-functions/add-product/bin
run_main seller-functions/register-seller/bin
run_main seller-functions/update-product/bin

printf "############## Courier... ##############\n\n"

set_current_root src/courier

run_main courier-main/bin

run_main courier-functions/register-courier/bin
run_main courier-functions/take-delivery/bin
run_main courier-functions/update-delivery-status/bin
run_main courier-functions/view-pending-orders/bin

printf "\n\n\n########## Script 'Run' Completato! ###########\n\n\n\n"
