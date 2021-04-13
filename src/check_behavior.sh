#!/bin/bash

LOG=./$(basename "$0" .sh).log

### functions ###

# Show and save results depending on the return code.
# $1: return code
# $2: output file name
save_res () {
    case "$1" in
        "0")
            echo "OK" | tee -a "$2"
            ;;
        "139")
            echo "NG (Seg fault)" | tee -a "$2"
            ;;
        "134")
            echo "NG (double free detected)" | tee -a "$2"
            ;;
        *)
            echo "NG" | tee -a "$2"
    esac
}

# Save the exe file name and dots to the log file
# $1: exe file name
# $2: output file name
save_name_and_dots () {
    echo -n "$1 " >> "$2"
    ## for j in $(seq $((30 - ${#1}))); do
    for ((i=0; i< $((30 - ${#1})); i++)); do
        echo -n "." >> "$2"
    done
    echo -n " " >> "$2"
}

# Show Environment
show_env () {
    uname -srvp
    cmake --version | head -n 1
    # TODO: synchronize with cmake
    if gcc --version > /dev/null 2>&1 ; then
        gcc --version | head -n 1
    fi
    if clang -v > /dev/null 2>&1 ; then
        clang -v | head -n 1
    fi
}

### main ###

rm -f "$LOG"
if [ -d ./build ] && [ "build" != "$(basename "$(pwd)")" ] ; then
    # If this script is executed not in and avobe ./build 
    cd ./build || exit
fi
echo "=== Checking Behavior ==="
for i in __check_*__ ; do    
    echo "=== $i ==="
    # show_expected "$i"
    ./"$i"
    # ./"$i" >> "$LOG" 2>&1
    # bash -c "bash -c ./$i" >>"$LOG" 2>&1 
    # bash -c "./$i >>$LOG 2>&1" 
    ret="$?"
    save_name_and_dots "$i" "$LOG"
    save_res "$ret" "$LOG"
    echo
done

echo "============= Summary of Behavior ============"
show_env
echo "----------------------------------------------"
cat "$LOG"

exit