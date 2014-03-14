#!/bin/sh

if [ $# -lt 3 ]; then
    echo "Usage: $0 csv_dir cpp_dir bin_dir [err|all]"
    exit 1
fi

csv2cpp()
{
    csv_dir=$1
    cpp_dir=$2
    bin_dir=$3
    flag=$4
    echo "csv=$csv_dir cpp=$cpp_dir bin=$bin_dir flag=$flag"

    csv_list=`ls $csv_dir/*.csv`
    for csv in $csv_list
    do
        if [ $csv != "$csv_dir/DropPack.csv" -a $csv != "$csv_dir/DropSubPack.csv" -a $csv != "$csv_dir/AtkDestation.csv" ]; then
            ./csv2cpp $csv $cpp_dir $bin_dir $flag
            #if [ $? -ne 0 ]; then
                #echo "Abort"
                #exit 1
            #fi
        fi
    done
}

csv2cpp $1 $2 $3 $4
