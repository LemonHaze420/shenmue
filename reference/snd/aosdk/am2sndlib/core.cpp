/*
	Wulinshu Team - 2021
		https://www.Wulinshu.com/

*/
#include "am2sndlib.h"
#include <cstdarg>

extern "C" {

#include "ao.h"
#include "eng_protos.h"
#include "mididump.h"
#include "wavedump.h"
	void flog(const char* format, ...) {
		FILE* file = ao_fopen("H:\\debug.log", "a");
		if (file) {
			char buffer[2048] = { '\0' };
			va_list args;
			va_start(args, format);
			vsprintf(buffer, format, args);
			va_end(args);
			fputs(buffer, file);
			fclose(file);
		}
	}
	/* file types */
	static uint32 type;
	static wavedump_t song_dump;
	volatile ao_bool ao_song_done;
	int SND_SUBTRACK = 0;
	int SND_SEQUENCES = 0;
	const char* gAicaDriverBuf;
	size_t gAicaDriverLen = 0;

	static struct
	{
		uint32 sig;
		const char* name;
		int32(*start)(uint8*, uint32);
		int32(*sample)(stereo_sample_t*);
		int32(*frame)(void);
		int32(*stop)(void);
		int32(*command)(int32, int32);
		uint32 rate;
		int32(*fillinfo)(ao_display_info*);
	} types[] =
	{
		{ 0x4454504B, "AM2 DTPK (.snd)", am2snd_start, dsf_sample, dsf_frame, dsf_stop, dsf_command, 60, am2snd_fill_info },

		{ 0xffffffff, "", NULL, NULL, NULL, NULL, NULL, 0, NULL }
	};

	/* ao_get_lib: called to load secondary files */
	 int ao_get_lib(const char* filename, uint8** buffer, uint64* length)
	{
		uint8* filebuf;
		uint32 size;
		FILE* auxfile;

		auxfile = ao_fopen(filename, "rb");
		if (!auxfile)
		{
			flog("Unable to find auxiliary file %s\n", filename);
			return AO_FAIL;
		}

		fseek(auxfile, 0, SEEK_END);
		size = ftell(auxfile);
		fseek(auxfile, 0, SEEK_SET);

		filebuf = (uint8*)malloc(size);

		if (!filebuf)
		{
			fclose(auxfile);
			flog("ERROR: could not allocate %d bytes of memory\n", size);
			return AO_FAIL;
		}

		fread(filebuf, size, 1, auxfile);
		fclose(auxfile);

		*buffer = filebuf;
		*length = (uint64)size;

		return AO_SUCCESS;
	}

	static void do_frame(unsigned long sample_count, stereo_sample_t* buffer)
	{
		unsigned long i;
		stereo_sample_t* p = buffer;
		for (i = 0; i < sample_count; i++)
		{
			(*types[type].sample)(p);
			p++;
		}
		wavedump_append(&song_dump, sample_count * sizeof(stereo_sample_t), buffer);
		(*types[type].frame)();
	}

	static void intr_handler(int sig)
	{
		ao_song_done = 1;
	}
	bool SNDLib::init(char* AicaBuf, size_t AicaLen)
	{
		if (!AicaBuf || AicaLen < 0)
			return false;

		gAicaDriverBuf = new  char[AicaLen];
		if (gAicaDriverBuf)
		{
			memset((void*)gAicaDriverBuf, 0, AicaLen);
			memcpy((void*)gAicaDriverBuf, AicaBuf, AicaLen);
			gAicaDriverLen = AicaLen;
		}
		
		return true;
	}
	bool SNDLib::open(std::string fileName, int subTrack)
	{
		if (fileName.empty())
			return false;

		filename = fileName;
		FILE* file = ao_fopen(filename.c_str(), "rb");
		if (!file) {
			flog("ERROR: could not open file %s\n", filename);
			return false;
		}

		// get the length of the file by seeking to the end then reading the current position
		uint32 size, filesig;
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		fseek(file, 0, SEEK_SET);
		fileBuffer = (uint8*)malloc(size);
		if (!fileBuffer) {
			fclose(file);
			flog("ERROR: could not allocate %d bytes of memory\n", size);
			return false;
		}
		fread(fileBuffer, size, 1, file);
		fclose(file);

		SND_SUBTRACK = subTrack;
		type = 0;

		filesig = fileBuffer[0] << 24 | fileBuffer[1] << 16 | fileBuffer[2] << 8 | fileBuffer[3];
		while (types[type].sig != 0xffffffff) {
			if (filesig == types[type].sig)	{
				break;
			} else {
				type++;
			}
		}

		// now did we identify it above or just fall through?
		if (types[type].sig != 0xffffffff) {
			flog("File identified as %s\n", types[type].name);
		}
		else
		{
			flog("ERROR: File is unknown, signature bytes are %02x %02x %02x %02x\n", fileBuffer[0], fileBuffer[1], fileBuffer[2], fileBuffer[3]);
			free(fileBuffer);
			return false;
		}

		if ((*types[type].start)(fileBuffer, size) != AO_SUCCESS) {
			free(fileBuffer);
			flog("ERROR: Engine rejected file!\n");
			return false;
		}

		numSequences = SND_SEQUENCES;
		bOpen = true;
		return true;
	}
	void SNDLib::stop() {
	}
	void SNDLib::play(int sampleCount) {
		if (bOpen) {
			sampleBuffer = new sample[sampleCount];
			memset((void*)sampleBuffer, 0x00, sampleCount  * sizeof sample);
			do_frame(sampleCount, (stereo_sample_t*)sampleBuffer);
		}
	}
	void SNDLib::close() {
		signal(SIGINT, SIG_IGN);
		wavedump_finish(&song_dump, 44100, 16, 2);
		free(fileBuffer);
		bOpen = false;
	}
}