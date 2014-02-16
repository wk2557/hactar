#!/bin/sh

cd $(dirname $_)

make && make cleanup

git add --all .
git commit -a

