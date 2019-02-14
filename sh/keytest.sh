#!/bin/bash 

GetKey() 
{ 
    aKey=(0 0 0) #定义一个数组来保存3个按键 
    
    cESC=`echo -ne "\033"` 
    cSpace=`echo -ne "\040"` 
    
    while : 
    do
	read -s -n 1 key #读取一个字符，将读取到的字符保存在key中 
	#echo $key 
	#echo XXX  
	
	aKey[0]=${aKey[1]} #第一个按键 
	aKey[1]=${aKey[2]} #第二个按键 
	aKey[2]=$key    #第三个按键 
	
	if [[ $key == $cESC && ${aKey[1]} == $cESC ]] 
	then
	    MyExit 
	elif [[ ${aKey[0]} == $cESC && ${aKey[1]} == "[" ]] 
	then
	    if [[ $key == "A" ]]; then echo KEYUP 
	    elif [[ $key == "B" ]]; then echo KEYDOWN 
	    elif [[ $key == "D" ]]; then echo KEYLEFT 
	    elif [[ $key == "C" ]]; then echo KEYRIGHT 
	    fi
	fi
    done
} 

GetKey
