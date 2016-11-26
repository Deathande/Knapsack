echo "Benchmarking knapsack"

for i in {1..20}
do
  value[$i]=$(./knapsack | grep Time | sed 's/Time of: //' | sed 's/ seconds//')
done

average=0

for v in ${value[@]}
do
  average=$(python -c "print($average + $v)")
done
average=$(python -c "print($average / 20)")
echo "$(date) average time of $average seconds"
echo "$(date) average time of $average seconds" >> timings.txt
