#PKG_CONFIG_PATH=/home/lixlian2/lianzi/opencvPackage/opencv-new-build/opencv-binary/lib/pkgconfig/:${PKG_CONFIG_PATH}
#export PKG_CONFIG_PATH
#export PKG_CONFIG_PATH=/home/lixlian2/lianzi/compiler/gcc-4.6.2-build/lib/pkgconfig/:${PKG_CONFIG_PATH}

#LD_LIBRARY_PATH=/home/lixlian2/lianzi/opencvPackage/opencv-new-build/opencv-binary/lib/:${LD_LIBRARY_PATH}
#export LD_LIBRARY_PATH

MY_COMPILER='g++'

fileName="MyMakefile"
rm -f ${fileName}

DEBUG=1



LINK_FLAG=""
if [ $DEBUG -eq 1 ]
then    
        LINK_FLAG="${LINK_FLAG}-g "
fi
LINK_FLAG="${LINK_FLAG}-o "
target="PicBlend"
OPENCV_LIB='`pkg-config --libs opencv`'
OPENCV_INCLUDE='`pkg-config --cflags opencv`'
OBJECTS=""

echo "`pkg-config --cflags opencv`"

list=`ls | grep ".cpp$"`
for (( i=0; i<${#list[@]}; i++))
do
    repText=`echo "${list[i]}" | sed "s/.cpp/.o/"`
	OBJECTS="${OBJECTS} ${repText}"
done
echo "OBJECT="${OBJECTS}" main.o" >> ${fileName}
echo '' >> ${fileName}
echo '' >> ${fileName}

echo "${target}: \${OBJECT}" >> ${fileName}
echo "	${MY_COMPILER} $LINK_FLAG ${target} *.o $OPENCV_LIB" >> ${fileName}
echo '' >> ${fileName}
echo '' >> ${fileName}

echo "`${MY_COMPILER} -MM main/main.cpp`" >> ${fileName}
echo "	${MY_COMPILER} -c $OPENCV_INCLUDE main/main.cpp" >> ${fileName}
echo '' >> ${fileName}

list=($list)
len=${#list[@]}
for (( i=0; i<${len}; i++ ))
do
	rely=`${MY_COMPILER} -MM ${list[i]}`
    rely=$(echo $rely | sed 's/\\//g' )
	echo ${rely} >> ${fileName}
	if [ $DEBUG -eq 1 ]
	then
		echo "	${MY_COMPILER} -c $OPENCV_INCLUDE ${list[i]}" >> ${fileName}
	fi
	echo ${i}
	echo "" >> ${fileName}
done


echo '' >> ${fileName}
echo '' >> ${fileName}
echo ".PHONY: clean" >> ${fileName}
echo "clean:" >> ${fileName}
echo "	rm *.o ${target}" >> ${fileName}

#make -f MyMakefile
