#!/bin/bash

echo "=== Nettoyage ==="
pkill -9 -f "intox_mrpiz"
pkill -9 -f "go"
rm -f go.txt

ROBOTSAuto=("MRPiZ yellow" "MRPiZ orange" "MRPiZ red" "MRPiZ purple" "MRPiZ magenta" "MRPiZ pink")
ROBOTSFile=("MRPiZ green" "MRPiZ sky-blue" "MRPiZ blue" "MRPiZ black" "MRPiZ grey")
ROBOTSManuel=("MRPiZ white")

PORT=12301

make clean
make

if [ ! -f "go" ]; then
    echo "Erreur de compilation. Arrêt."
    exit 1
fi

for robot in "${ROBOTSAuto[@]}"; do
    env WEBOTS_ROBOT_NAME="$robot" WEBOTS_CONTROLLER_NAME="$robot" WEBOTS_CONTROLLER_URL="tcp://host.docker.internal:1234/$robot" ../webots_mrpiz_v0.6.1/controllers/intox_mrpiz-x86_64-linux-gnu/intox_mrpiz-x86_64-linux-gnu $PORT &
    sleep 0.1
    ./go $PORT auto &
    PORT=$((PORT + 1))
done

for robot in "${ROBOTSFile[@]}"; do
    env WEBOTS_ROBOT_NAME="$robot" WEBOTS_CONTROLLER_NAME="$robot" WEBOTS_CONTROLLER_URL="tcp://host.docker.internal:1234/$robot" ../webots_mrpiz_v0.6.1/controllers/intox_mrpiz-x86_64-linux-gnu/intox_mrpiz-x86_64-linux-gnu $PORT &
    sleep 0.1
    ./go $PORT file &
    PORT=$((PORT + 1))
done

sleep 0.5
touch go.txt

for robot in "${ROBOTSManuel[@]}"; do
    env WEBOTS_ROBOT_NAME="$robot" WEBOTS_CONTROLLER_NAME="$robot" WEBOTS_CONTROLLER_URL="tcp://host.docker.internal:1234/$robot" ../webots_mrpiz_v0.6.1/controllers/intox_mrpiz-x86_64-linux-gnu/intox_mrpiz-x86_64-linux-gnu $PORT &
    sleep 0.1
    ./go $PORT manual
    PORT=$((PORT + 1))
done

wait