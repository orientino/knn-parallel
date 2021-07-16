# This script run the program and its arguments given in input in $1 for $2 iterations.
#
# Usage example: ./run_single.sh "knn_seq 5" 5

if [ $# -ne 2 ]; then
    echo './run_single.sh "[program] [args]" iterations'
	exit 1
fi

# loop to perform $2 independent runs
for((i=0; i<$2; i++)); do
        ./$1; 
    done | awk -v n=$2 '{sum += $4; count += 1; print "Iteration " count " time (usec): " $4} END {print "Average time (usec) over " n " iterations: " sum/n}'
