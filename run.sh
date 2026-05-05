#!/bin/bash

ADRESSE_WEBOTS="host.docker.internal"

pkill -9 -f "intox_mrpiz"
pkill -9 -f "go"
rm -f go.txt

ROBOTSFile=("MRPiZ yellow" "MRPiZ orange" "MRPiZ red" "MRPiZ purple" "MRPiZ magenta")
ROBOTSAuto=("MRPiZ pink" "MRPiZ green" "MRPiZ sky-blue" "MRPiZ blue" "MRPiZ black" "MRPiZ grey")
ROBOTSManuel=("MRPiZ white")

PORT=12301

make clean
make

if [ ! -f "go" ]; then
    echo "Erreur : La compilation a échoué. Arrêt du script."
    exit 1
fi

lancer_robot() {
    local nom_robot="$1"
    local mode="$2"
    local port_robot=$3
    local arriere_plan=$4

    env WEBOTS_ROBOT_NAME="$nom_robot" WEBOTS_CONTROLLER_NAME="$nom_robot" WEBOTS_CONTROLLER_URL="tcp://${ADRESSE_WEBOTS}:1234/$nom_robot" ../webots_mrpiz_v0.6.1/controllers/intox_mrpiz-x86_64-linux-gnu/intox_mrpiz-x86_64-linux-gnu $port_robot &

    sleep 0.1

    if [ "$arriere_plan" == "true" ]; then
        ./go $port_robot $mode &
    else
        ./go $port_robot $mode
    fi
}

for robot in "${ROBOTSAuto[@]}"; do
    lancer_robot "$robot" "auto" $PORT "true"
    PORT=$((PORT + 1))
done

for robot in "${ROBOTSFile[@]}"; do
    lancer_robot "$robot" "file" $PORT "true"
    PORT=$((PORT + 1))
done

sleep 0.5

for robot in "${ROBOTSManuel[@]}"; do
    lancer_robot "$robot" "manual" $PORT "false"
    PORT=$((PORT + 1))
done

wait