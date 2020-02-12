#!/bin/bash
mkdir -p test_cases/in test_cases/out
for i in {17..20}
do
	wget https://judgedat.u-aizu.ac.jp/testcases/GRL_2_A/$i/in -O test_cases/in/$i.txt
	sleep 3
	wget https://judgedat.u-aizu.ac.jp/testcases/GRL_2_A/$i/out -O test_cases/out/$i.txt
	sleep 3
done

