rm -rf ./perf_measures
mkdir perf_measures
for f in executables/*
do
	echo ${f/*\/}
	for i in {1..500}
		do
			#sudo perf record stat $f
			#sudo perf script -i perf.data > ./perf_measures/$f$i.txt
			sudo perf stat $f 2>&1 >/dev/null | grep task-clock | awk {'print $1'} >> "./perf_measures/et_${f/*\/}.txt"
		done
	
done
