#!/bin/bash

red='\033[0;31m'
light_red='\033[1;31m'
green='\033[0;32m'
light_green='\033[1;32m'
yellow='\033[1;33m'
white='\033[0;37m'
nocolor='\033[0m'
echo -e -n "${light_green}"
printf "%0.s*" {1..100}
echo -e "${nocolor}"
