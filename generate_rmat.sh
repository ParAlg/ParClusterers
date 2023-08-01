#!/bin/bash
cp RMAT.cc external/gbbs/utils/generators/RMAT.cc
cd external/gbbs/utils/generators/
echo "in external/gbbs/utils/generators/"
make

# nums=(262144 1048576 4194304)
# edge_nums=(1000000000) #1,000,000,000
nums=(64)
edge_nums=(10)
output_dir="/home/sy/datasets/RMAT"
for num in ${nums[@]}
do
  for edge_num in ${edge_nums[@]}
  do
    echo "numactl -i all ./RMAT -n $num -m $edge_num -outfile $output_dir/rmat_$num\_$edge_num"
    numactl -i all ./RMAT -n $num -m $edge_num -outfile $output_dir/rmat_$num\_$edge_num
  done
done

