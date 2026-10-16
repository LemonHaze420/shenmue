#pragma once

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#define DLL_EXPORT __declspec(dllexport) 

class SNDLib {
public:
	SNDLib() = default;
	~SNDLib() = default;
	DLL_EXPORT bool init(char* AicaBuf, size_t AicaLen);
	DLL_EXPORT bool open(std::string fileName, int subTrack = 0);
	DLL_EXPORT void play(int sampleCount = (44100 / 60));
	DLL_EXPORT void close();
	DLL_EXPORT void stop();

public:
	std::string filename;
	bool bOpen				= false;
	int numSequences		= -1;
	int selectedSequence	= -1;

	struct sample {
		int16_t l;
		int16_t r;
	};
	sample * sampleBuffer;

protected:
	unsigned char * fileBuffer;
};