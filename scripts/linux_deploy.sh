#!/bin/bash
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -b|--bin)
    BIN="$2"
    shift # past argument
    shift # past value
    ;;
    --qmlDir)
    QMLDIR="$2"
    shift # past argument
    shift # past value
    ;;
    --qmake)
    QMAKE="$2"
    shift # past argument
    shift # past value
    ;;
    --targetDir)
    TARGETDIR="$2"
    shift # past argument
    shift # past value
    ;;
esac
done

echo "BIN = ${BIN}"
echo "QMLDIR = ${QMLDIR}"
echo "QMAKE = ${QMAKE}"
echo "TARGETDIR = ${TARGETDIR}"

cqtdeployer -bin ${BIN} -qmlDir ${QMLDIR} -qmake ${QMAKE} -targetDir ${TARGETDIR}
rm -rf ${TARGETDIR}/librrcore.so.1
ln -s ${TARGETDIR}/lib/librrcore.so ${TARGETDIR}/librrcore.so.1

#cqtdeployer -bin rr,rrcore/librrcore.so -qmlDir ../../RecordRackDesktop/src/qml/ -qmake ~/Qt/5.13.1/gcc_64/bin/qmake -targetDir ../install-root/
#ln -s ../install-root/lib/librrcore.so ../install-root/librrcore.so.1
