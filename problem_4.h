#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <stdio.h>

DWORD WINAPI thread_fun(LPVOID param);
int problem_4();
bool check_palindrome(std::string str);
