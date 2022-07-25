#!/usr/bin/bash

export _OS_NAME=$1
export BIN_PATH=$2

cat GRUB_TEMPLATE | envsubst > "$3"