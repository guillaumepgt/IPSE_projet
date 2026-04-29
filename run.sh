#!/bin/bash

pkill -9 -f intox_mrpiz
pkill -9 -f "go"
rm -f go.txt

ROBOTS=(
    "MRPiZ yellow"
)

PORT_BASE=12301

make clean
make

for i in "${!ROBOTS[@]}"
do
    ROBOT_NAME="${ROBOTS[$i]}"
    PORT=$((PORT_BASE + i))

    env WEBOTS_ROBOT_NAME="$ROBOT_NAME" WEBOTS_CONTROLLER_NAME="$ROBOT_NAME" WEBOTS_CONTROLLER_URL="tcp://host.docker.internal:1234/$ROBOT_NAME" ../webots_mrpiz_v0.6.1/controllers/intox_mrpiz-x86_64-linux-gnu/intox_mrpiz-x86_64-linux-gnu $PORT &

    sleep 0.05

    ./go $PORT
done

touch go.txt
