@echo off
make tests
tests.exe
tests.exe > testlog.txt
start testlog.txt