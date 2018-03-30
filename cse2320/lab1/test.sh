#!/bin/bash

expected="$(cat samples/$1.out)"
actual="$(cat samples/$1.in | ./main.o)"

if [ "$actual" == "$expected" ]; then
  echo "Test $1...succeeded"
else
  echo "Test $1...failed!!"
  echo "expected:"
  echo "$expected"
  echo
  echo "actual:"
  echo "$actual"
  echo
  echo
fi

