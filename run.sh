#!/bin/sh
set -eu
cd "$(dirname "$0")"
if [ ! -r /proc/cpuinfo ]; then
    echo 'Linux /proc/cpuinfo is required' >&2
    exit 1
fi
model=$(awk -F: '/^model name/ { if (match($2, /i[0-9]+-[0-9A-Za-z]+/)) print substr($2, RSTART, RLENGTH); exit }' /proc/cpuinfo)
if [ -z "$model" ]; then
    echo 'Could not extract CPU model' >&2
    exit 1
fi
n=${1:-100000}
echo "CPU from /proc/cpuinfo: $model"
echo "N: $n"
printf '%s\n%s\n' "$n" "$model" | ./main
