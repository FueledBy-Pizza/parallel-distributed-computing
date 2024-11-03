#!/bin/zsh

execute_controlled_command() {
  "$@" &
  local background_pid=$!
  wait $background_pid

  local exit_status=$?
  if [ $exit_status -ne 0 ]; then
    echo "Error: exited with non-zero status $exit_status"
    exit
  fi
}

if [ $# -ne 1 ]
then
  echo "Usage: $0 <main.tex>"
  exit 1
fi

filename=$(echo "$1" | sed 's/\..*//')

execute_controlled_command pdflatex --shell-escape --file-line-error --synctex=1 $1
execute_controlled_command biber $filename
execute_controlled_command makeglossaries $filename
execute_controlled_command pdflatex --shell-escape --file-line-error --synctex=1 $1
execute_controlled_command pdflatex --shell-escape --file-line-error --synctex=1 $1
