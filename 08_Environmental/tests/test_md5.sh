echo "abcd" > input
md5sum input | cut -d ' ' -f 1 > output
echo -n -e "\n" >> output
echo 'MD5 "abcd' | ./rhasher | cut -d ' ' -f 2 | cmp output
rm input output
