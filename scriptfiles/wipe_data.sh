#/bin/sh

for i in `ls -d Open-GTO/*/; ls -d Open-GTO/*/*/`
do
	rm -f $i*
done