#!/bin/bash
# tests/verify.sh

PROGRAM=$1
INPUT_FILE=$2
REF_FILE=$3

if [[ ! -f "$PROGRAM" ]]; then
    echo "ERROR: executable not found: $PROGRAM"
    exit 1
fi

if [[ ! -f "$INPUT_FILE" ]]; then
    echo "ERROR: input file not found: $INPUT_FILE"
    exit 1
fi

if [[ ! -f "$REF_FILE" ]]; then
    echo "ERROR: reference file not found: $REF_FILE"
    exit 1
fi


OUTPUT=$(mktemp)


"$PROGRAM" < "$INPUT_FILE" > "$OUTPUT" 2>&1

expected=$(cat "$REF_FILE" | tr -d '\r\n' | xargs)
got=$(cat "$OUTPUT" | tr -d '\r\n' | xargs)

if [ "$expected" = "$got" ]; then
    rm -f "$OUTPUT"
    exit 0
else
    echo "FAIL: Output does not match $REF_FILE"
    echo "--- Expected:"
    cat "$REF_FILE"
    echo "--- Got:"
    cat "$OUTPUT"
    rm -f "$OUTPUT"
    exit 1
fi
