# This script run the program and its arguments given in input in $1 for $2 iterations.
#
# Usage: ./run_single.sh "./program arg1 arg2 ..." iterations

if [ $# -ne 2 ]; then
    echo 'Usage: ./run_single.sh "./program arg1 arg2 ..." iterations'
	exit 1
fi

# loop to perform $2 independent runs
for((i=0; i<$2; i++)); do
        $1; 
    done | awk -v n=$2 '{sum += $4; count += 1; print "Iteration " count " time (msec): " $4} END {print "Average time (msec) over " n " iterations: " sum/n}'
