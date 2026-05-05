#!/bin/bash

pkill -9 -f "go"
rm -f go.txt

ROBOTSFile=("MRPiZ yellow" "MRPiZ orange" "MRPiZ red" "MRPiZ purple" "MRPiZ magenta")
ROBOTSAuto=("MRPiZ pink" "MRPiZ green" "MRPiZ sky-blue" "MRPiZ blue" "MRPiZ black" "MRPiZ grey")
ROBOTSManuel=("MRPiZ white")

PORT=12301

make clean
make

if [ ! -f "go" ]; then
    echo "Erreur de compilation. Arrêt."
    exit 1
fi

for robot in "${ROBOTSFile[@]}"; do
    echo "Connexion de $robot au port $PORT (FILE)"
    ./go $PORT file &
    PORT=$((PORT + 1))
done

for robot in "${ROBOTSAuto[@]}"; do
    echo "Connexion de $robot au port $PORT (AUTO)"
    ./go $PORT auto &
    PORT=$((PORT + 1))
done

sleep 0.5

for robot in "${ROBOTSManuel[@]}"; do
    ./go $PORT manual
    PORT=$((PORT + 1))
done

wait