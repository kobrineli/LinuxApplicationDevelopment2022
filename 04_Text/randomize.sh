#!/bin/bash

DELAY=0
if [[ $# != 0 ]]
then
    DELAY=$1
fi

lines=()
poses=()
cur=1
while IFS= read -r line;
do
    lines+=("$line")
    for j in $(seq 1 ${#line});
    do
        poses+=("$cur.$j")
    done
    cur=$((cur + 1))
done

tput clear

for pos in $(shuf -e "${poses[@]}"); do
    sleep $DELAY
    line_idx=$(echo $pos | cut -d '.' -f 1)
    col_idx=$(echo $pos | cut -d '.' -f 2)

    symbol=${lines[(($line_idx - 1))]:$((col_idx - 1)):1}
    if [[ $symbol != " " ]]
    then
        tput cup $((line_idx - 1)) $((col_idx - 1))
        printf "$symbol"
    fi
done

tput cup $(($(tput lines) - 1)) 0
