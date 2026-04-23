#!/bin/bash
INPUT=$(cat)
COMMAND=$(echo "$INPUT" | jq -r '.command // ""')

if echo "$COMMAND" | grep -qE 'rm -rf /|DROP TABLE|DELETE FROM .* WHERE 1|git push --force'; then
  echo "⛔ Blocked: potentially destructive command detected." >&2
  exit 2
fi

exit 0
