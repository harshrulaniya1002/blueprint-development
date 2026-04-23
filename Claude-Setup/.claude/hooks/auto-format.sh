#!/bin/bash
INPUT=$(cat)
FILE=$(echo "$INPUT" | jq -r '.path // ""')

if echo "$FILE" | grep -qE '\.(md|json|yml|yaml)$'; then
  if command -v prettier >/dev/null 2>&1; then
    prettier --write "$FILE" >/dev/null 2>&1 || true
  fi
fi

exit 0
