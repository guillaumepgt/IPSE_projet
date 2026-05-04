#!/bin/bash

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
    echo "Connexion de $robot au port $PORT (AUTO)"
    ./go $PORT auto &
    PORT=$((PORT + 1))
done

for robot in "${ROBOTSFile[@]}"; do
    echo "Connexion de $robot au port $PORT (FILE)"
    ./go $PORT file &
    PORT=$((PORT + 1))
done

sleep 0.5
touch go.txt

for robot in "${ROBOTSManuel[@]}"; do
    ./go $PORT manual
    PORT=$((PORT + 1))
done

wait