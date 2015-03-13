echo "#nodeNum,nodeCount,edgeCount,density=edgeCount/nodeCount"
for num in ../random_topo_files/*; do
  edges=0
  nodes=0
  for file in `eval echo $num"/*"`; do
    echo -en "\r\e[K Parsing: "$file" (NodeCount: "$nodes" EdgeCount: "$edges")";
    for i in $(seq 1 100); do
      nodes=$((nodes + 1))
      count=$(cat $file | grep -E "(\("$i",|\s"$i"\))" | wc -l)
      edges=$((edges + count))
    done;
  done;
  num="'"$num"'"
  num=$(python -c "print $num[$num.index('iles/')+len('iles/'):]")
  echo -en "\r\e[K"
  echo $num" "$nodes" "$edges" "$(python -c "print $edges.0/$nodes")
done;
