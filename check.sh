#/bin/bash

gcc matmul.c -o matmul.x && ./matmul.x 1 > noop
./matmul.x 2 > doop
diff doop noop > /dev/null
C=$?
if [ $C -eq 0 ]
then
  echo "No Diff"
else
  echo "Diff"
fi

