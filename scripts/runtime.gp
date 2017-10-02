reset
set xlabel 'numerical distribution'
set ylabel 'time(cycles)'
set title 'performance comparison'
set term png enhanced font 'Verdana,10'
set output 'runtime.png'
set format x "%10.0f"
set xtic 2000
set xtics rotate by 45 right

plot [:][:500]'iteration.txt' using 1:2 with points title 'iteration',\
'byte.txt' using 1:2 with points title 'byte',\
'binary.txt' using 1:2 with points title 'binary',\
'recursive.txt' using 1:2 with points title 'recursive',\
'harley.txt' using 1:2 with points title 'harley'

