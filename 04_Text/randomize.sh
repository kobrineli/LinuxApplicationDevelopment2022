#!/bin/bash

exit_handler() {
    trap - EXIT
    test -r "$TMPFILE" && rm -f "$TMPFILE"
    test -r "$POSFILE" && rm -f "$POSFILE"
}

trap exit_handler EXIT

DELAY=0
if [[ $# != 0 ]]
then
    DELAY=$1
fi

TMPFILE="`mktemp`"
while IFS= read -r VAR;
do
    echo -e "$VAR"
    echo -e "$VAR" >> $TMPFILE
done

LINES="$(cat $TMPFILE | wc -l)"

POSFILE="`mktemp`"
for i in $(seq 1 $LINES);
do
    LINE=$(sed -n "${i}p" $TMPFILE)
    for j in $(seq 1 ${#LINE});
    do
        echo "$i $j" >> $POSFILE
    done
done

#for pos in "$(shuf $POSFILE)"
#do
#    echo "${positions[0]} "
#done

tput clear

while IFS= read -r pos
do
    sleep $DELAY
    position=($pos)
    line_idx=${position[0]}
    col_idx=${position[1]}

    LINE=$(sed -n "${line_idx}p" $TMPFILE)

    tput cup $((line_idx - 1)) $((col_idx - 1))
    echo -n -e "${LINE:$((col_idx - 1)):${col_idx}}"
done < <(shuf $POSFILE)

tput cup $(($(tput lines) - 1)) 0
