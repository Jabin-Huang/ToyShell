a=5
b=9
#注释

if [[$a>$b]]
then 
    echo a>b
    while [[$a>$b]]
    do
        a=$((${a}-1))
        echo $a
	    echo WHILE_executed!
    done
else
    echo a<b
    until [[$a>=$b]]
    do 
        b=$((${b}-1))
        echo $b
        echo UNTIL_executed!
    done
fi

for v in one two three four
do
    echo $v
done