#/bin/sh

for i in `ls -d Open-GTO/*/; ls -d Open-GTO/*/*/`
do
	echo $i
	rm -f $i*
done