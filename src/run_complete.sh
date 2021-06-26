# This bash script lauches the input program $1 and tests it with increasing parallelism degree in a loop up to 'par'.
# Each iteration is an average of 'iter' number of independent runs.
#
# Usage: ./run_complete.sh ./program

if [ $# -ne 1 ]; then
    echo "Usage: ./run_complete.sh ./program"
	exit 1
fi

par=256		# maximum parallelism degree
iter=5		# number of iterations to reduce variance
k=100		# number of neighbors to compute

# loop to increase the parallelism degree
for((i=1; i<$par+1; i++)); do
	# loop for multiple independent iterations to reduce variance
	for((j=0; j<$iter; j++)); do
		$1 $i $k; 
		done | awk -v iter=$iter -v par=$i '{sum += $4} END {print sprintf("%s,%s", par, sum/iter)}'
		# done | awk -v n=$iter '{sum += $4; count += 1; print "Iteration " count " time (msec): " $4} END {print "Average time (msec) over " n " iterations: " sum/n}'
    done
