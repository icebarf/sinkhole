#!/bin/sh
if echo "$1" | grep -Eq -- '-elf($|-)'; then
  echo 0
  exit
fi
echo 1