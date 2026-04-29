#!/bin/bash

echo "=== Nettoyage des anciens processus ==="
pkill -9 -f "go"
rm -f go.txt
sleep 0.5

ROBOTS=(
    "MRPiZ yellow"
)

PORT_BASE=12301

make clean
make

for i in "${!ROBOTS[@]}"
do
    PORT=$((PORT_BASE + i))

    ./go $PORT
done

sleep 0.5

touch go.txt

wait