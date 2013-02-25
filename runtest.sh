#!/bin/bash

d=`dirname $0`

$d/pybuildext.sh

python "$d/test/wait.py" &
bg0=$!

python "$d/test/wait.py" &
bg1=$!

sleep 1

python "$d/test/send.py"

kill $bg0
kill $bg1
