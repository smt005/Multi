#include "SampleClass.h"
#include <fstream>
#include <iostream>

SampleClass::SampleClass()
{
	
}

SampleClass::~SampleClass()
{
	
}


void SampleClass::main()
{
    
}

//#pragma once static

void SampleClass::showText(char* text)
{
    std::cout << text << std::endl;
}

void SampleClass::stopFunction(char* text)
{
    std::cout << text << std::endl;
    
    char chars[256];
    std::cin >> chars;
    
    std::cout << "ENTER: " << chars << std::endl;
}
