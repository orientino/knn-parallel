# This bash script lauches the input program $1 and tests it with increasing parallelism degree in a loop up to 'par'.
# Each iteration is an average of 'iter' number of independent runs.
#
# Usage example: ./run_complete.sh knn_par 5 256 5

if [ $# -ne 4 ]; then
    echo "./run_complete.sh [program] [k] [max_nw] [iter]"
	exit 1
fi

k=$2		# number of neighbors to compute
par=$3		# maximum parallelism degree
iter=$4		# number of iterations to reduce variance

echo "nw,average"

# loop to increase the parallelism degree
for((i=1; i<$par; i+=2)); do
	# loop for multiple independent iterations to reduce variance
	for((j=0; j<$iter; j++)); do
		./$1 $k $i; 
		done | awk -v iter=$iter -v par=$i '{sum += $4} END {print sprintf("%s,%s", par, sum/iter)}'
		# done | awk -v n=$iter '{sum += $4; count += 1; print "Iteration " count " time (msec): " $4} END {print "Average time (msec) over " n " iterations: " sum/n}'
    done
