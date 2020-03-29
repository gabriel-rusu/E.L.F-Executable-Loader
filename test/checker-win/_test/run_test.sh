#!/bin/bash

#
# Tema 3 - Loader de Executabile - Test Suite
#
# 2018, Operating Systems
#

# ----------------- General declarations and util functions ------------------ #

loader_main="_test/run_test"
MAIN_TEST_DIR="_test/outputs"
INPUT_DIR="_test/inputs"
REFS_DIR="_test/refs"
LOG_FILE="/dev/null"
max_points=95
TEST_TIMEOUT=30

# change this of you want to keep the logs after execution
DO_CLEANUP=${DO_CLEANUP:-yes}

TEST_LIB=_test/test_lib.sh
CHECK_SKIP='_test/inputs'

# load the lib functions
if ! [ -e "$TEST_LIB" ]; then
	echo "Test library not found. Check \$TEST_LIB ($TEST_LIB)"
	exit 1
fi
source "$TEST_LIB"

# ---------------------------------------------------------------------------- #

# ----------------- Init and cleanup tests ----------------------------------- #

# Initializes a test
init_test()
{
	OUT_F=$MAIN_TEST_DIR"/test"$test_index".out"
	TMP_F=$MAIN_TEST_DIR"/test"$test_index".tmp"
	REF_F=$REFS_DIR"/test"$test_index".ref"
	FAULT_CNT_F=$MAIN_TEST_DIR"/test"$test_index"_fault_cnt.out"
}

# Cleanups a test
cleanup_test()
{
	[ "$DO_CLEANUP" = "yes" ] && rm -f $OUT_F $TMP_F &> /dev/null
}

# Initializes the whole testing environment
# Should be the first test called
init_world()
{
	[ -d $MAIN_TEST_DIR ] && rm -rf $MAIN_TEST_DIR
	mkdir -p $MAIN_TEST_DIR
}

# Cleanups the whole testing environment
# Should be the last test called
cleanup_world()
{
	[ "$DO_CLEANUP" = "yes" ] && rm -rf $MAIN_TEST_DIR
}

# ---------------------------------------------------------------------------- #

# ----------------- Test Suite ----------------------------------------------- #

get_recorded_faults()
{
	if [ -f "$FAULT_CNT_F" ]; then
		echo $(($(cat "$FAULT_CNT_F")))
	else
		echo 0
	fi
}


# Tests if a program exits with success
test_success()
{
	init_test

	bin="$1"
	num_faults=$2

	LD_LIBRARY_PATH=. "$loader_main" "$bin" "$FAULT_CNT_F" &> $OUT_F
	RUN_RC=$?

	recorded_faults=$(get_recorded_faults)

	basic_test test $RUN_RC -eq 0 -a $num_faults -eq $recorded_faults

	if [ $RUN_RC -ne 0 ]; then
		echo "Incorrect exit code: got $RUN_RC, expected 0"
	fi

	if [ $recorded_faults -ne $num_faults ]; then
		echo "Incorrect number of faults: got $recorded_faults, expected $num_faults"
	fi

	cleanup_test
}

# Tests if a program crashes with SIGSEGV
test_segv()
{
	init_test

	bin="$1"
	num_faults=$2

	# use { } to supress the "Segmentation fault" message
	{ LD_LIBRARY_PATH=. "$loader_main" "$bin" "$FAULT_CNT_F" &> $OUT_F; } 2> /dev/null
	RUN_RC=$?

	recorded_faults=$(get_recorded_faults)

	basic_test test $RUN_RC -eq 139 -a $num_faults -eq $recorded_faults

	if [ $RUN_RC -ne 139 ]; then
		echo "Incorrect exit code: got $RUN_RC, expected 139"
	fi

	if [ $recorded_faults -ne $num_faults ]; then
		echo "Incorrect number of faults: got $recorded_faults, expected $num_faults"
	fi

	cleanup_test
}

test_ref()
{
	init_test

	bin="$1"
	num_faults=$2
	op=$3
	[ -z "$op" ] && op="-eq"

	LD_LIBRARY_PATH=. "$loader_main" "$bin" "$FAULT_CNT_F" &> $OUT_F
	RUN_RC=$?

	recorded_faults=$(get_recorded_faults)

	compare $OUT_F $REF_F > $TMP_F
	DIFF_RC=$?

	basic_test test $DIFF_RC -eq 0 -a $RUN_RC -eq 0 -a $recorded_faults $op $num_faults
	[ -s "$TMP_F" ] && cat "$TMP_F"

	if [ $RUN_RC -ne 0 ]; then
		echo "Incorrect exit code: got $RUN_RC, expected 0"
	fi

	if [ ! $recorded_faults $op $num_faults ]; then
		if [ "$op" = "-gt" ]; then
			op_str=">"
		fi
		echo "Incorrect number of faults: got $recorded_faults, expected $op_str $num_faults"
	fi

	cleanup_test
}

test_exec()
{
	test_success "_test/inputs/check_exec.exe" 3
}

test_hello()
{
	test_ref "_test/inputs/hello.exe" 4
}

test_sum()
{
	test_ref "_test/inputs/sum.exe" 5
}

test_bss()
{
	test_ref "_test/inputs/bss.exe" 4
}

test_data_bss()
{
	test_ref "_test/inputs/data_bss.exe" 5
}

test_no_perm()
{
	test_segv "_test/inputs/no_perm.exe" 4
}

test_inv_perm()
{
	test_segv "_test/inputs/inv_perm.exe" 4
}

test_perms()
{
	test_ref "_test/inputs/check_perm.exe" 7
}

test_qsort()
{
	test_ref "_test/inputs/qsort.exe" 7
}

# ---------------------------------------------------------------------------- #

# ----------------- Run test ------------------------------------------------- #

test_fun_array=(							\
	test_coding_style	"Sources check"			5	\
	test_exec		"Test simple execution"		5	\
	test_hello		"Test hello"			10	\
	test_sum		"Test sum"			10	\
	test_bss		"Test bss"			10	\
	test_data_bss		"Test data and bss"		10	\
	test_no_perm		"Test no permissions"		10	\
	test_inv_perm		"Test invalid permissions"	10	\
	test_perms		"Test permissions"		10	\
	test_qsort		"Test complex program"		15	\
)
# ---------------------------------------------------------------------------- #

# ----------------- Run test ------------------------------------------------- #

# first we check if we have everything defined
check_tests

# run tests
run_tests $@

exit 0
