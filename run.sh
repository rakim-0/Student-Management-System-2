make clear
make -B all

rm output.txt

echo "DEPORTES: Football Club"  >> output.txt
echo "-----------------------" >> output.txt
echo "Scenario 1:" >> output.txt
echo "" >> output.txt
./kernel >> output.txt
echo "" >> output.txt
time1=$(awk '{ sum += $1 } END { print sum/1.0 }' log.txt);
rm log.txt

echo "Scenario 2:" >> output.txt
echo "" >> output.txt
echo "Increased Memory by 20%" >> output.txt
sed -i -e 's/count >= 5/count >= 6/g' main_memory.c
make clear
make -B all
./kernel >> output.txt
time2=$(awk '{ sum += $1 } END { print sum }' log.txt);
rm log.txt

echo "" >> output.txt
echo "Increased Memory by 50%" >> output.txt
sed -i -e 's/count >= 6/count >= 9/g' main_memory.c
make clear
make -B all
./kernel >> output.txt
time3=$(awk '{ sum += $1 } END { print sum}' log.txt);
rm log.txt

sed -i -e 's/count >= 9/count >= 5/g' main_memory.c

echo "" >> output.txt
echo "Scenario 3:" >> output.txt 
echo "" >> output.txt
echo "Original times: $time1 $time2 $time3" >> output.txt
echo "Updated times: $((time1 / 2)) $((time2 / 2)) $((time3 / 2))" >> output.txt
echo ""
cat output.txt
