# !/bin/sh
./dir2key db db-key >> all_step.build
./createcsv db-key data/csvkey >> all_step.build
sh divisioncsv.sh data/csvkey data/csvkey.train data/csvkey.test >> all_step.build
echo $(wc -l data/csvkey.train | cut -d' ' -f 1) | cat - data/csvkey.train > temp && mv temp data/csvkey.train
echo $(wc -l data/csvkey.test | cut -d' ' -f 1) | cat - data/csvkey.test > temp && mv temp data/csvkey.test
./key4kmeans data/csvkey.train data/key_train.kmeans >> all_step.build
./key4kmeans data/csvkey.test data/key_test.kmeans >> all_step.build
./kmeans -k 1500 -i 30 -b 10000 data/key_train.kmeans data/kmeans_train.voc >> all_step.build
./kmeans -k 1500 -i 30 -b 10000 data/key_test.kmeans data/kmeans_test.voc >> all_step.build
./assign data/csvkey.train data/kmeans_train.voc data/table.train >> all_step.build
./assign data/csvkey.test data/kmeans_test.voc data/table.test >> all_step.build
./table2libsvm data/csvkey.train data/table.train data/libsvm.train >> all_step.build
./table2libsvm data/csvkey.test data/table.test data/libsvm.test >> all_step.build
./svm-train -s 0 -c 512 -t 2 -g 0.0005 -e 0.1 data/libsvm.train >> all_step.build
./svm-predict data/libsvm.test libsvm.train.model data/libsvm.out >> all_step.build
