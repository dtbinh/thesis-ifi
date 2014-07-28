# !/bin/sh
#./dir2key data/scenes-15 data/scenes-15-key
#./createcsv data/scenes-15-key data/scenes-15-csvkey key
#echo $(wc -l data/scenes-15-csvkey | cut -d' ' -f 1) | cat - data/scenes-15-csvkey > temp && mv temp data/scenes-15-csvkey
#./key4kmeans data/scenes-15-csvkey data/scenes-15-key.kmeans
./kmeans -k 500 -i 30 -b 10000 data/scenes-15-key.kmeans data/scenes-15-kmeans.voc
./assign data/scenes-15-csvkey data/scenes-15-kmeans.voc data/scenes-15-table
./table2libsvm data/scenes-15-csvkey data/scenes-15-table data/scenes-15-libsvm
sh divisioncsv.sh data/scenes-15-libsvm data/scenes-15-libsvm-d500.train data/scenes-15-libsvm-d500.test
#./svm-train -s 0 -c 512 -t 2 -g 0.0005 -e 0.1 data/scenes-15-libsvm.train
#./svm-predict data/scenes-15-libsvm.test caltech-7-libsvm.train.model data/scenes-15-libsvm.out
