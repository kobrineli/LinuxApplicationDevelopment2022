echo "abcd" > input
sha1sum input | cut -d ' ' -f 1 > output
echo -n -e "\n" >> output
echo 'SHA1 "abcd' | ./rhasher | cut -d ' ' -f 2 | cmp output
rm input output
