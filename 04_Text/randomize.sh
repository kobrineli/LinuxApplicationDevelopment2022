#!/bin/bash

DELAY=0
if [[ $# != 0 ]]
then
    DELAY=$1
fi

lines=()
poses=()
cur=1
linewidth=0
while IFS= read -r line;
do
    if [[ cur -eq "1" ]]
    then
        linewidth=${#line}
    fi
    lines+=("$line")
    for j in $(seq 1 ${#line});
    do
        poses+=("$cur.$j")
    done
    cur=$((cur + 1))
done

tput clear

linesnum=$(($(tput lines) / 2))
colsnum=$(($(tput cols) / 2))
colormax=$(tput colors)

for pos in $(shuf -e "${poses[@]}"); do
    sleep $DELAY
    line_idx=$(echo $pos | cut -d '.' -f 1)
    col_idx=$(echo $pos | cut -d '.' -f 2)
    symbol=${lines[(($line_idx - 1))]:$((col_idx - 1)):1}
    if [[ $symbol != " " ]]
    then
        color=$(shuf -i $((colormax - 90))-$((colormax * 7 / 10)) -n1)
        tput setaf $color
        tput cup $((line_idx - 1 + linesnum - cur / 2)) $((col_idx - 1 + colsnum - linewidth / 2))
        printf "$symbol"
    fi
done

tput cup $(($(tput lines) - 1)) 0
