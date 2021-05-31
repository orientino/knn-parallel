for((i=0; i<10; i++)); do ./knn 5 5; done | awk '{sum += $4} END {print sum/10}'
