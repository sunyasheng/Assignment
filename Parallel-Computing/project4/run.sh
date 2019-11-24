HADOOP_CMD="/usr/local/hadoop/bin/hadoop"
STREAM_JAR_PATH="/usr/local/hadoop/share/hadoop/tools/lib/hadoop-streaming-2.9.0.jar"

MAPPER_PATH="/home/hadoop/homework/Parallel-Computing/project4/src/mapper.py"
COMBINER_PATH="/home/hadoop/homework/Parallel-Computing/project4/src/combiner.py"
REDUCER_PATH="/home/hadoop/homework/Parallel-Computing/project4/src/reducer.py"
DATA_PATH="/home/hadoop/homework/Parallel-Computing/project4/src/data_generater.py"
INPUT_DIR="project4/data/Temperature.txt"
OUTPUT_DIR="project4/output/"

python $DATA_PATH
$HADOOP_CMD dfs -rm -r -f $OUTPUT_DIR

$HADOOP_CMD jar  $STREAM_JAR_PATH \
-file  $MAPPER_PATH -mapper "python $MAPPER_PATH" \
-file $REDUCER_PATH -reducer "python $REDUCER_PATH" \
-file $COMBINER_PATH -combiner "python $COMBINER_PATH" \
-input $INPUT_DIR -output $OUTPUT_DIR