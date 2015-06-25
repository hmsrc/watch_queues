#!/bin/bash
. /opt/lsf/conf/profile.lsf 
cd /home/rc200/util/efficiency

mkdir -p data

/opt/ritg-util/bin/lsf_cpuslots > data/e1.txt

awk '{
if($1 != "User" && $5 > 0 ){
    print $1
}
}' data/e1.txt | sort | uniq -c | awk '{print $2,$1}'| sort -n -k 2,2 -r > data/e2.txt

while read -r c1 c2
do
echo Hi "$c1": > data/m.txt
echo " " >> data/m.txt

cat data/m01.txt >> data/m.txt
echo " " >> data/m.txt

awk -v var="$c1" '{
if( $1 == var  ){
    print $3
}
}' data/e1.txt | sort >> data/m.txt
echo " " >> data/m.txt

cat data/m02.txt >> data/m.txt

mail -s "Your jobs on orchestra " "$c1",rc@hms.harvard.edu < data/m.txt 

done < data/e2.txt 

