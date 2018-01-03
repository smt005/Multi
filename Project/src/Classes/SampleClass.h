#ifndef SampleClass_H
#define SampleClass_H

class SampleClass
{
public:
	SampleClass();
	~SampleClass();

	virtual void main();
    
    static void showText(char* text);
	static void stopFunction(char* text);
};

#endif
