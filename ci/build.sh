#!/bin/bash

function ShowUsage()
{
    cat <<EOT
$(basename $0) options
    [--builddir=<builddir>]  - name of build directory
    [--cxx=<path/to/cxx>]    - path for CXX environment variable
    [--cc=<path/to/cc>]      - path for CC env variable
    [--cmake=<options>]      - option string to pass to CMake
EOT
    return 0    
}

BUILDDIR=build
PARAM_CC=
PARAM_CXX=
PARAM_CMAKE=

while test $# -gt 0; do
    param="$1"
    if test "${1::1}" = "-"; then
        if test ${#1} -gt 2 -a "${1::2}" = "--" ; then
            param="${1:2}"
        else
            param="${1:1}"
        fi
    else
        break
    fi

    shift

    case $param in
    builddir=*)
        BUILDDIR=$(echo $param|cut -f2 -d'=')
        ;;
    cc=*)
        PARAM_CC=$(echo $param|cut -f2 -d'=')
        ;;
    cxx=*)
        PARAM_CXX=$(echo $param|cut -f2 -d'=')
        ;;
    cmake=*)
        PARAM_CMAKE=$(echo $param|cut -f2- -d'=')
        ;;
    help|h|?|-?)
        ShowUsage
        exit 0
        ;;
    *)
        echo "Error: Unknown parameter: $param"
        ShowUsage
        exit 2
        ;;    
    esac
done

echo "BUILDDIR=$BUILDDIR"
echo "PARAM_CC=$PARAM_CC"
echo "PARAM_CXX=$PARAM_CXX"
echo "PARAM_CMAKE=$PARAM_CMAKE"

[ ! -d ./config-cpp-git ] && { echo "ERROR: repo not cloned!"; exit 1; }

# Change to the base directory of the repo
cd config-cpp-git

# Create build directory and switch to it
mkdir -p $BUILDDIR
cd $BUILDDIR

# Configure via CMake
if [ -n "$PARAM_CXX" -o -n "$PARAM_CC" ]; then
    # Override CC and CXX
    CC=$PARAM_CC CXX=$PARAM_CXX cmake $PARAM_CMAKE -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=ON ..
else
    cmake $PARAM_CMAKE -Dconfig_CPP_BUILD_EXAMPLE=OFF -Dconfig_CPP_BUILD_TESTS=ON ..
fi
ret=$?
[ $ret -ne 0 ] && exit $ret

# Build
make
ret=$?
[ $ret -ne 0 ] && exit $ret

[ ! -x ./test/ConfigCppTests ] && { echo "ERROR: unit tests not built!"; exit 1; }

# Run unit tests
./test/ConfigCppTests
ret=$?

# Return result
exit $ret
