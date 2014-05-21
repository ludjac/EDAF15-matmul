#/bin/bash

gcc matmul.c -o matmul.x && ./matmul.x 1 > noop
./matmul.x 2 > doop2
./matmul.x 3 256 > doop3
diff doop2 noop > /dev/null
C=$?
if [ $C -eq 0 ]
then
  echo "No Diff for opt-2"
else
  echo "Diff for opt-2"
fi

diff doop3 noop > /dev/null
C=$?
if [ $C -eq 0 ]
then
  echo "No Diff for opt-3"
else
  echo "Diff for opt-3"
fi
