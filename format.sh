#! /bin/env bash
shopt -s globstar
for f in **/*.c; do
    clang-format -i $f
done

for f in **/*.h; do
    clang-format -i $f
done