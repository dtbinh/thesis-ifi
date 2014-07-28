# !/bin/sh
rm -fr $2 $3
i=0
while read line
do
	if [ `expr $i % 2` -eq 0 ]
	then
		echo "$line" >> $2
	else
		echo "$line" >> $3
	fi
	i=`expr $i + 1`
done <$1
