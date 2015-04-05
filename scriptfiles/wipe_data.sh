#/bin/sh

for i in `ls -d open-gto/*/; ls -d open-gto/*/*/`
do
	rm -f $i*
done