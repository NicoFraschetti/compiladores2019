#!/bin/bash

if [ ! -f a.out ]; then
	sh script.sh
fi
input_file_name=$2
assembly_file_name=$(echo "$input_file_name" | cut -f1 -d".")
assembly_file_name="${assembly_file_name}.s"
exec_name=$1;
input_linked_files=""
if test -f $assembly_file_name; then
  rm $assembly_file_name
fi
./a.out $input_file_name
while [ "$3" != "" ]; do
    input_linked_files="${input_linked_files}$3 "
    shift
done
input_linked_files=${input_linked_files%?}
if test -f $exec_name; then
  rm $exec_name
fi
if test -f "$assembly_file_name"; then 
  gcc -o $exec_name $assembly_file_name $input_linked_files
  ./$exec_name
fi
