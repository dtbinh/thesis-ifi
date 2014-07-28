#! /bin/bash

#
# author: PHAM nguyen Khang
# Description: create data for k-means
# 

if [ $# -lt 2 ] ; then
	echo "Usage: $0 <SIFT-DIR> <RESULT-FILE> [size]"
	exit
fi

SIFT_DIR=$1
RESULT=$2

TMP1=$RESULT.tmp1
TMP2=$RESULT.tmp2
TMP3=$RESULT.tmp3
TMP4=$RESULT.tmp4

rm -f $TMP1
touch $TMP1

for FILE in `find $SIFT_DIR -mindepth 1 -name *.sift`; do
	if [ -f $FILE ] ; then
		echo $FILE >> $TMP1
	fi
done

if [ -z $3 ] ; then
	TOTAL=`cat $TMP1 | wc -l`
	echo total = $TOTAL
	SIZE=$((TOTAL))
else
	SIZE=$3
fi

awk '{printf("%.8f %s\n", rand(), $0);}' $TMP1 > $TMP2

sort -n $TMP2 | head -$SIZE | cut -d" " -f2 > $TMP3


#Concatenation
echo Concatenating...
rm -f $TMP4
for FILE in `cat $TMP3` ; do
	sed '1,2 d' $FILE >> $TMP4
done

LINE=`cat $TMP4 | wc -l`
echo $LINE 128 > $RESULT
cat $TMP4 >> $RESULT

#Cleaning
rm -f $TMP1 $TMP2 $TMP3 $TMP4
