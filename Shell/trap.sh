#!/bin/bash

func()
{
    echo "hello"
}

# func INT
trap func 2
while true
do
    n=0
done