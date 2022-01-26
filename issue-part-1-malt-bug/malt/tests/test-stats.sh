#!/bin/bash

# configure bash
set -e

# set vars
SOURCE_DIR=${1}
EXE="./test-main"
PROF_OK="./test-malt-ok.json"
PROF_CURRENT="./test-malt-current.json"

#colors
RESET="\x1b[0m"
RED="\x1b[31m"
GREEN="\x1b[32m"
MAGENTA="\x1b[35m"
# build the code
echo -e "${MAGENTA}=========================== Search exe ==========================${REST}"
if ! test -f ${EXE} ; then
	echo -e "${RED}Fail to find ${EXE}${RESET}"
	exit 1
fi

# run valid malt
echo -e "${MAGENTA}========================= Running reference =====================${RESET}"
if [ -f "${SOURCE_DIR}/tests/test-stats-ref.json" ]; then
	echo "Already ran !"
	cp ${SOURCE_DIR}/tests/test-stats-ref.json ${PROF_OK}
else
	~/malt-ok/bin/malt -o "output:name=${PROF_OK};output:indent=true" ${EXE}
	cp ${PROF_OK} "${SOURCE_DIR}/tests/test-stats-ref.json"
fi
echo

# run local malt
echo -e "${MAGENTA}=========================== Running current ====================${RESET}"
LD_PRELOAD="../src/lib/libmalt.so" MALT_OPTIONS="output:name=${PROF_CURRENT};output:indent=true" ${EXE}

# extract readable stacks
echo
echo -e "${MAGENTA}=========================== Extract stacks ======================${RESET}"
node ${SOURCE_DIR}/src/webview/extract_stats.js -i ${PROF_OK} -f "call_a" | awk "matched; /============================================================/ { matched = 1 }" > ${PROF_OK}.stack
node ${SOURCE_DIR}/src/webview/extract_stats.js -i ${PROF_CURRENT} -f "call_a" | awk "matched; /============================================================/ { matched = 1 }" > ${PROF_CURRENT}.stack
echo "--------------------- Expected (call_a stats) -------------------"
cat ${PROF_OK}.stack
echo "--------------------- Obtained (call_a stats) -------------------"
cat ${PROF_CURRENT}.stack

# check
echo -e "${MAGENTA}=========================== Result =============================${RESET}"
echo
if diff -q  ${PROF_OK}.stack  ${PROF_CURRENT}.stack > /dev/null 2>/dev/null; then
	echo -e "${GRREN}>>>>>>>>>>>>> OK <<<<<<<<<<<<<<${RESET}"
else
	echo -e "${RED}>>>>>>>>>>>>> NOT OK <<<<<<<<<<<<<<${RESET}"
	exit 1
fi
