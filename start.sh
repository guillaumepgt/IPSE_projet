#!/bin/bash

echo "=== Nettoyage des anciens processus ==="
pkill -9 -f "go"
rm -f go.txt
sleep 0.5

ROBOTS=(
    "MRPiZ_yellow"
    "MRPiZ_orange"
    "MRPiZ_red"
    "MRPiZ_purple"
    "MRPiZ_pink"
    "MRPiZ_white"
    "MRPiZ_black"
    "MRPiZ_blue"
)

PORT_BASE=12301

make clean
make

for i in "${!ROBOTS[@]}"
do
    PORT=$((PORT_BASE + i))

    ./go $PORT &
done

sleep 0.5

touch go.txt

wait