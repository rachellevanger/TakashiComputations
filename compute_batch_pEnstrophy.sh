
# Rename argument variables
OUTPUT_DIR=$1
FLOAT_DIR=$2
P=$3

CUR_DIR=$(pwd)

cd $OUTPUT_DIR

> ./enstrophy_calc.txt

# Parse file_list.txt for list of files to process
cat ./orderOfFiles.txt| while read LINE
do

        LINE_TEXT=( $LINE )
        FILE=${LINE_TEXT[0]}

        # Compute p-enstrophy on each file and output the file name and resulting enstrophy value in a file.
        STROUT="$($CUR_DIR/pWeightedEnstrophyTakashiFiles $FLOAT_DIR/$FILE $P)"
        echo $FILE $STROUT >> ./enstrophy_calc.txt

done

