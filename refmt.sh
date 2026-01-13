#!/usr/bin/env bash

# ============================================================
# C++ Source Formatter
# ------------------------------------------------------------
# ✔ Formats C/C++ source files using clang-format.
# ✔ Scans ONLY the ./src directory (relative to this script).
# Created by: Yuriy Dubinin, ScaleTrade.
# ============================================================

set -euo pipefail

# ------------------------------------------------------------
# ANSI colors
# ------------------------------------------------------------
COLOR_INFO='\033[0;36m'
COLOR_OK='\033[0;32m'
COLOR_WARN='\033[0;33m'
COLOR_ERR='\033[0;31m'
COLOR_RESET='\033[0m'

# ------------------------------------------------------------
# Resolve directories
# ------------------------------------------------------------
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC_DIR="${SCRIPT_DIR}/src"

# ------------------------------------------------------------
# File extensions to format
# ------------------------------------------------------------
EXTENSIONS=(
  "*.c"
  "*.cc"
  "*.cpp"
  "*.cxx"
  "*.h"
  "*.hh"
  "*.hpp"
)

# ------------------------------------------------------------
# Preconditions
# ------------------------------------------------------------
if [[ ! -d "${SRC_DIR}" ]]; then
  echo -e "${COLOR_ERR} ❌ [refmt]: src/ directory not found near script${COLOR_RESET}"
  exit 1
fi

if ! command -v clang-format >/dev/null 2>&1; then
  echo -e "${COLOR_ERR} ❌ [refmt]: clang-format is not installed${COLOR_RESET}"
  exit 1
fi

# ------------------------------------------------------------
# Formatting process
# ------------------------------------------------------------
echo -e "${COLOR_INFO} 🔧 [refmt] Starting C++ code formatting${COLOR_RESET}"
echo -e "${COLOR_INFO} 📁 [refmt]: Target directory: ${SRC_DIR}${COLOR_RESET}"

formatted_count=0

for ext in "${EXTENSIONS[@]}"; do
  while IFS= read -r -d '' file; do
    echo -e "${COLOR_INFO} ✏️${COLOR_RESET} $(basename "$file")"
    clang-format -i "$file"
    ((formatted_count++))
  done < <(
    find "${SRC_DIR}" \
      -type d -name "vcpkg" -prune -o \
      -type f -name "$ext" -print0
  )
done

# ------------------------------------------------------------
# Summary
# ------------------------------------------------------------
echo -e "${COLOR_OK} ✅ [refmt]: Formatting complete${COLOR_RESET}"
echo -e "${COLOR_OK} 📄 Files formatted: ${formatted_count}${COLOR_RESET}"
