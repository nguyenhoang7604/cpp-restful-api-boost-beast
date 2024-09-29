#!/usr/bin/env bash

LOCAL_SOURCE_PATH=$(pwd)
nCPUs=$(getconf _NPROCESSORS_ONLN)
nBUILD_JOBs=$(( ${nCPUs} + 1))
START_TIME=$(date -u "+%s")

PREFIX_PATH=${1}
BUILD_OUTPUT="${PREFIX_PATH}/build.log"
PREFIX_SOURCE_PATH=${PREFIX_PATH}/sources

CMAKE_OPTIONS+=" -DCMAKE_C_COMPILER=${ccache}"
CMAKE_OPTIONS+=" -DCMAKE_C_COMPILER_ARG1=${CC}"
CMAKE_OPTIONS+=" -DCMAKE_CXX_COMPILER=${ccache}"
CMAKE_OPTIONS+=" -DCMAKE_CXX_COMPILER_ARG1=${CXX}"

CMAKE_OPTIONS+=" -DCMAKE_INSTALL_LIBDIR=/lib"
CMAKE_OPTIONS+=" -DCMAKE_INSTALL_PREFIX=/usr"

RED="\e[1;31m"
GREEN="\e[1;32m"
NORMAL="\e[0;39m"
TAB="\e[64G"

ASTERISK="${GREEN}*${NORMAL}"
echo=$(which echo 2>/dev/null || echo echo)

function print_status()
{
	${echo} -en ${TAB}
	[ $1 -eq 0 ] \
		&& ${echo} -ne "[  ${GREEN}OK${NORMAL}  ]\n" \
		|| { ${echo} -ne "[ ${RED}FAIL${NORMAL} ]\n" ; exit $1 ; }
}

#============================
# arg 0 do_cmake_build
# arg 1 source path
function do_cmake_build()
{
	local source_path=${1}
	local build_path=${source_path}
	local ret=0

	echo "executing: make -C ${build_path} -j ${nBUILD_JOBs}" >>${BUILD_OUTPUT}
	VERBOSE=${VERBOSE} make -C ${build_path} -j ${nBUILD_JOBs} >>${BUILD_OUTPUT} 2>&1
	ret=${?}
	echo "make returns: ${ret}" >>${BUILD_OUTPUT}

	return ${ret}
}

function do_cmake_install()
{
	local source_path=${1}
	local build_path=${source_path}
	local ret=0

	echo "executing: make -C ${build_path} -j ${nBUILD_JOBs} install DESTDIR=${PREFIX_PATH}" >>${BUILD_OUTPUT}
	VERBOSE=${VERBOSE} make -C ${build_path} -j ${nBUILD_JOBs} install DESTDIR=${PREFIX_PATH} >>${BUILD_OUTPUT} 2>&1
	ret=${?}
	echo "make install returns: ${ret}" >>${BUILD_OUTPUT}

	return ${ret}
}

#============================
function configure_local()
{
	${echo} -ne " ${ASTERISK} Configuring:"

	local build_path=${PREFIX_SOURCE_PATH}
	local cmake_options="${CMAKE_OPTIONS} ${CMAKE_EXTRA_OPTIONS}"
	local source_path=${LOCAL_SOURCE_PATH}
	local ret=0

	[ ! -d "${build_path}" ] || ( rm -rf ${build_path} && mkdir -p ${build_path} )

	CFLAGS="-g -Os"
	CPPFLAGS="-g -Os"
	CXXFLAGS="-g -Os"
	LDFLAGS=""

	echo "executing: CFLAGS=\"${CFLAGS}\" CPPFLAGS=\"${CPPFLAGS}\" CXXFLAGS=\"${CXXFLAGS}\" LDFLAGS=\"${LDFLAGS}\" cmake -B ${build_path} -S ${source_path} ${cmake_options}" >>${BUILD_OUTPUT}
	CFLAGS="${CFLAGS}" \
	CPPFLAGS="${CPPFLAGS}" \
	CXXFLAGS="${CXXFLAGS}" \
	LDFLAGS="${LDFLAGS}" \
	cmake -B ${build_path} -S ${source_path} ${cmake_options} >>${BUILD_OUTPUT} 2>&1
	ret=${?}
	echo "cmake returns: ${ret}" >>${BUILD_OUTPUT}
	print_status ${ret}
}

function build_local()
{
	${echo} -ne " ${ASTERISK} Building:"
	do_cmake_build ${PREFIX_SOURCE_PATH}
	print_status ${?}
}

function install_local()
{
	${echo} -ne " ${ASTERISK} Installing:"
	do_cmake_install ${PREFIX_SOURCE_PATH}
	print_status ${?}
}

function do_init()
{
	${echo} -ne " ${ASTERISK} Cleaning up: "
	rm -rf ${PREFIX_PATH}
	mkdir -p ${PREFIX_SOURCE_PATH}
	print_status ${?}
}

function do_done()
{
	EndTime=$(date -u "+%s")

	if [ -n "${START_TIME}" ] && [ -n "${EndTime}" ]; then
		BuildTime=$(date -u -d "0 ${EndTime} sec - ${START_TIME} sec" "+%H:%M:%S")
		echo -e " Build took: ${BuildTime}"
	fi
}

do_init
configure_local
build_local
install_local
do_done
