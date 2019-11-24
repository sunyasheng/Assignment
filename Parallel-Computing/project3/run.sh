# cat ./data/Gone_With_The_Wind.txt | src/mapper.py | sort | src/reducer.py
HADOOP_CMD="/usr/local/hadoop/bin/hadoop"
STREAM_JAR_PATH="/usr/local/hadoop/share/hadoop/tools/lib/hadoop-streaming-2.9.0.jar"
MAPPER_PATH="/home/hadoop/homework/Parallel-Computing/project3/src/mapper.py"
REDUCER_PATH="/home/hadoop/homework/Parallel-Computing/project3/src/reducer.py"
COMBINER_PATH="/home/hadoop/homework/Parallel-Computing/project3/src/combiner.py"
INPUT_DIR="project3/data/Gone_With_The_Wind.txt"
OUTPUT_DIR="project3/output/"

$HADOOP_CMD dfs -rm -r -f $OUTPUT_DIR

$HADOOP_CMD jar  $STREAM_JAR_PATH \
-file  $MAPPER_PATH -mapper "python $MAPPER_PATH" \
-file $REDUCER_PATH -reducer "python $REDUCER_PATH" \
-file $COMBINER_PATH -combiner "python $COMBINER_PATH" \
-input $INPUT_DIR -output $OUTPUT_DIR