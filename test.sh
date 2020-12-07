#!/bin/sh

counter=0
while [ "$counter" -lt 100 ]
  do
    build/client
    counter=`expr $counter + 1`
  done
