#! /bin/sh
testFiles=$(ls *.um $2)
for testFile in $testFiles ; do
    #testName=$(echo $testFile | sed -E ’s/(.*).um/\1/’)
    testName=${testFile%.um}
    echo $testName
done

for testFile in $testFiles ; do
    testName=${testFile%.um}
    if [ -f $testName.gt ] ; then
        umdump $testFile >> $testName.dump
    else
        echo $testName has no output!
    fi
done

#outputFile=umlab.c

#if [ -f $outputFile ] ; then
#echo "exists!"
#else
#echo "does not exist!"
#fi

