#!/bin/dash

ONLY_PRINT=0

for ARG in "$@"
do
	case $ARG in
		--only-print)
			ONLY_PRINT=1
			shift
			;;
	esac
done

if [ $# -ne 2 ]
then
	echo "Missing parameters" 1>&2
	echo "Usage:" 1>&2
	echo "\t$0 [OPTION] EXEC_DIR GRAPH_DIR" 1>&2
	echo "" 1>&2
	echo "Option:" 1>&2
	echo "\t--only-print just print commands to standard output" 1>&2
	exit 1
fi

EXEC_DIR=$1
GRAPH_DIR=$2

EXPERIMENT_LIST=$(find $EXEC_DIR -type f -executable -name "xp_*")
GRAPH_LIST=$(find $GRAPH_DIR -name *.graph)

FACTOR_LIST="10 1000"
THRESHOLD_LIST="10 50"

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

run_experiment() {
	GRAPH=$1
	EXPERIMENT=$2

	shift 2

	OTHER_ARGS="$(echo "$@" | tr -s ' ' '-')-"

	LOG_FILE="log/$(basename $EXPERIMENT)-$OTHER_ARGS$(basename ${GRAPH%.*}).log"
	timeout 12h $EXPERIMENT $GRAPH "$@" > $LOG_FILE 2>&1
	if [ $? -eq 124 ]
	then
		echo "\t$RED$EXPERIMENT $GRAPH "$@" timeout$NC"
		rm $LOG_FILE
	else
		echo "\t$GREEN$EXPERIMENT $GRAPH "$@" done$NC"
	fi
}

print_experiment() {
	GRAPH=$1
	EXPERIMENT=$2

	shift 2

	OTHER_ARGS="$(echo "$@" | tr -s ' ' '-')-"

	LOG_FILE="log/$(basename $EXPERIMENT)-$OTHER_ARGS$(basename ${GRAPH%.*}).log"
	echo "timeout 12h $EXPERIMENT $GRAPH "$@" > $LOG_FILE 2>&1"
}

mkdir -p log

for GRAPH in $GRAPH_LIST
do
	if [ $ONLY_PRINT -ne 1 ]
	then
		echo "graph name: $(basename $GRAPH)"
	fi

	for EXPERIMENT in $EXPERIMENT_LIST
	do
		if [ "$(basename $EXPERIMENT)" = "xp_approx" ]
		then
			for FACTOR in $FACTOR_LIST
			do
				if [ $ONLY_PRINT -eq 1 ]
				then
					print_experiment $GRAPH $EXPERIMENT $FACTOR
				else
					run_experiment $GRAPH $EXPERIMENT $FACTOR
				fi
			done
		elif [ "$(basename $EXPERIMENT)" = "xp_approx_threshold" ]
		then
			for FACTOR in $FACTOR_LIST
			do
				for THRESHOLD in $THRESHOLD_LIST
				do
					if [ $ONLY_PRINT -eq 1 ]
					then
						print_experiment $GRAPH $EXPERIMENT $FACTOR $THRESHOLD
					else
						run_experiment $GRAPH $EXPERIMENT $FACTOR $THRESHOLD
					fi
				done
			done
		else
			if [ $ONLY_PRINT -eq 1 ]
			then
				print_experiment $GRAPH $EXPERIMENT
			else
				run_experiment $GRAPH $EXPERIMENT
			fi
		fi
	done
done
