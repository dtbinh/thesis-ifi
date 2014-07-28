# !/bin/sh
./dir2key data/scenes data/scenes-key
./createcsv data/scenes-key data/scenes-csvkey key
echo $(wc -l data/scenes-csvkey | cut -d' ' -f 1) | cat - data/scenes-csvkey > temp && mv temp data/scenes-csvkey
./key4kmeans data/scenes-csvkey data/scenes-key.kmeans
./kmeans -k 1500 -i 30 -b 10000 data/scenes-key.kmeans data/scenes-kmeans.voc
./assign data/scenes-csvkey data/scenes-kmeans.voc data/scenes-table
./table2libsvm data/scenes-csvkey data/scenes-table data/scenes-libsvm
sh divisioncsv.sh data/scenes-libsvm data/scenes-libsvm.train data/scenes-libsvm.test
#./svm-train -s 0 -c 512 -t 2 -g 0.0005 -e 0.1 data/scenes-libsvm.train
#./svm-predict data/scenes-libsvm.test caltech-7-libsvm.train.model data/scenes-libsvm.out
