#!/bin/sh
set -eu
cd "$(dirname "$0")"
if ! command -v strace >/dev/null 2>&1; then
    echo 'Install strace first' >&2
    exit 1
fi
model=$(awk -F: '/^model name/ { if (match($2, /i[0-9]+-[0-9A-Za-z]+/)) print substr($2, RSTART, RLENGTH); exit }' /proc/cpuinfo)
if [ -z "$model" ]; then
    echo 'Could not extract CPU model' >&2
    exit 1
fi
input=$(mktemp)
plain=$(mktemp)
buffered=$(mktemp)
trap 'rm -f "$input" "$plain" "$buffered"' EXIT
printf '100000\n%s\n' "$model" > "$input"
strace -e read -o "$plain" ./main < "$input" >/dev/null
strace -e read -o "$buffered" ./main --buffered < "$input" >/dev/null
printf 'Unbuffered read(0) calls: %s\n' "$(grep -c '^read(0,' "$plain" || true)"
printf 'Buffered read(0) calls: %s\n' "$(grep -c '^read(0,' "$buffered" || true)"
