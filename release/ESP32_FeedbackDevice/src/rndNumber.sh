while :
do

curl --location --request POST "http://192.168.116.22/post?hum=$(shuf -i 20-90 -n 1)&temp=$(shuf -i 0-50 -n 1)" 

sleep 1

done