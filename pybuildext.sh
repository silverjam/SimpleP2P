#!/bin/bash

d=`dirname $0`

python "$d/setup.py" build_ext --inplace
mv simplep2p.so "$d/test"
