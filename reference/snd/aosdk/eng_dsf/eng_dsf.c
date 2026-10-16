//
// Audio Overload
// Emulated music player
//
// (C) 2000-2008 Richard F. Bannister
//

//
// eng_dsf.c
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ao.h"
#include "eng_protos.h"
#include "corlett.h"
#include "dc_hw.h"
#include "aica.h"

#define DK_CORE	(1)

#if DK_CORE
#include "arm7.h"
#else
#include "arm7core.h"
#endif

extern int SND_SUBTRACK;
extern int SND_SEQUENCES;
extern char* gAicaDriverBuf;
extern size_t gAicaDriverLen;

static corlett_t	c = {0};

int dsf_lib(int libnum, uint8 *lib, uint64 size, corlett_t *c)
{
	// patch the file into ram
	uint32 offset = lib[0] | lib[1]<<8 | lib[2]<<16 | lib[3]<<24;
	memcpy(&dc_ram[offset], lib+4, size-4);

	return AO_SUCCESS;
}

int32 dsf_start(uint8 *buffer, uint32 length)
{
	// clear Dreamcast work RAM before we start scribbling in it
	memset(dc_ram, 0, 8*1024*1024);

	// Decode the current SSF
	if (corlett_decode(buffer, length, &c, dsf_lib) != AO_SUCCESS)
	{
		return AO_FAIL;
	}

	#ifdef DEBUG
	{
		FILE *f;

		f = ao_fopen("H:\\dcram.bin", "wb");
		fwrite(dc_ram, 2*1024*1024, 1, f);
		fclose(f);
	}
	#endif

	#if DK_CORE
	ARM7_Init();
	#else
	arm7_init(0, 45000000, NULL, NULL);
	arm7_reset();
	#endif
	dc_hw_init();

	return AO_SUCCESS;
}


uint32 ReadUint(uint8* buffer, uint32 offset)
{
    uint32 retval = 0;
    for (int i = 3; i >= 0; i--)
    {
        retval <<= 8;
        retval += buffer[i + offset];
    }
    return retval;
}

uint16 ReadUShort(uint8* buffer, uint32 offset)
{
    uint16 retval = 0;
    for (int i = 1; i >= 0; i--)
    {
        retval <<= 8;
        retval += buffer[i + offset];
    }
    return retval;
}

void WriteUint(uint8* buffer, uint32 offset, uint32 value)
{
    for (int i = 0; i < 4; i++)
    {
        buffer[offset + i] = (value & 0xFF);
        value >>= 8;
    }
}
#include <stdbool.h>
void unfuck(uint8* buffer, uint32 length) {
    //For the PC port of Shenmue I & II, d3t decompressed all of the ADPCM
    //samples in the file without changing the compression and frequency
    //flags to denote the change. They also converted them to 32-bit PCM.
    //This converts the file back into a Dreamcast compatible format.
    uint32 sampleTable = ReadUint(buffer, 0x3C);
    uint32 sampleCount = ReadUint(buffer, sampleTable) + 1;
    for (uint32 i = 0; i < sampleCount; i++)
    {
        uint32 sampleEntryStart = sampleTable + 4 + (i * 0x10);
        uint32 info = ReadUint(buffer, sampleEntryStart);
        bool compressed = ((info & 0x1000000) == 0x1000000);
        uint16 loopEnd = ReadUShort(buffer, sampleEntryStart + 6);
        uint32 sampleSize = ReadUint(buffer, sampleEntryStart + 12);
        if ((sampleSize >> 1) > loopEnd)
        {
            //Size >> 1 is the total sample count on DC
            //LoopEnd is always <= Sample count. If LoopEnd is > Sample count,
            //we probably have 32 bit samples meaning we are the PC version
            uint32 location = info & 0x7FFFFF;
            uint16 loopStart = ReadUShort(buffer, sampleEntryStart + 4);
            info = info & ~0x1000000; //Clear compression flag
            info = info | 0x2800000; //Set Frequency flags
            WriteUint(buffer, sampleEntryStart, info);

            //Cut sample size in half:
            WriteUint(buffer, sampleEntryStart + 12, sampleSize >> 1);
            for (uint32 j = 0; j < sampleSize >> 2; j++) {
                //Convert 32-bit samples to 16-bit by shifting them.
                buffer[location + (j * 2)] = buffer[location + (j * 4) + 1];
                buffer[location + (j * 2) + 1] = buffer[location + (j * 4) + 3];
            }
        }
    }
}
int32 am2snd_start(uint8 *buffer, uint32 length)
{
    // clear Dreamcast work RAM before we start scribbling in it
    memset(dc_ram, 0, 8*1024*1024);

    //Load AICADRV.BIN starting at 0
        // Copy driver into memory why is every line commented btw
    memcpy(dc_ram, gAicaDriverBuf, gAicaDriverLen);

    uint8* buf = (uint8*)malloc(length);
    memcpy(buf, buffer, length);
    // unfuck(buf, length);


    //Copy SND file into memory
    memcpy(dc_ram + 0x10000, buf, length);

    //Copy DTPK ID from SND file
    dc_ram[0x60] = buf[0x4];
    dc_ram[0x61] = buf[0x5];
    dc_ram[0x62] = buf[0x6];
    dc_ram[0x63] = buf[0x7];

    //Bank 0 command
    dc_ram[0x400] = 0xA0;
    dc_ram[0x401] = 0x00;
    dc_ram[0x402] = 0x11;
    dc_ram[0x403] = 0x00;

    //Find Sequence offset
    int sequenceOffset = buf[0x2C] + (buf[0x2D] << 8) + (buf[0x2E] << 16);
    if (sequenceOffset == 0) {
        printf("SND file does not contain any tracks");
        return AO_FAIL;
    }
    int sequences = buf[sequenceOffset]; //Should always be 0 for what we want.
    //It's a 32-bit number, but if there are > 10 sequences in a file that would be odd.
    if (sequences == 0 && buf[sequenceOffset + 8] != 0) {
        sequences = buf[sequenceOffset + 8]; //Last subtrack index?
    }
    int trackToPlay = SND_SUBTRACK;
    SND_SEQUENCES = sequences;
    if (trackToPlay > sequences) {
        printf("The selected track number is greater than the number of tracks in this file");
        return AO_FAIL;
    }

    //Play sequence (track) command
    dc_ram[0x404] = buf[sequenceOffset + 4 + 3];
    dc_ram[0x405] = buf[sequenceOffset + 4 + 2];
    dc_ram[0x406] = trackToPlay; //Track
    dc_ram[0x407] = 0;

    //Set length to endless
    corlett_length_set(0, 0);

#if DK_CORE
ARM7_Init();
#else
arm7_init(0, 45000000, NULL, NULL);
arm7_reset();
#endif
dc_hw_init();

    return AO_SUCCESS;
}

int32 dsf_sample(stereo_sample_t *sample)
{
	#if DK_CORE
	ARM7_Execute((33000000 / 60 / 4) / 735);
	#else
	arm7_execute((33000000 / 60 / 4) / 735);
	#endif
	AICA_Update(NULL, NULL, sample);
	corlett_sample_fade(sample);

	return AO_SUCCESS;
}

int32 dsf_frame(void)
{
	return AO_SUCCESS;
}

int32 dsf_stop(void)
{
	return AO_SUCCESS;
}

int32 dsf_command(int32 command, int32 parameter)
{
	switch (command)
	{
		case COMMAND_RESTART:
			return AO_SUCCESS;

	}
	return AO_FAIL;
}

int32 dsf_fill_info(ao_display_info *info)
{
	info->title[1] = "Name: ";
	info->info[1] = corlett_tag_lookup(&c, "title");

	info->title[2] = "Game: ";
	info->info[2] = corlett_tag_lookup(&c, "game");

	info->title[3] = "Artist: ";
	info->info[3] = corlett_tag_lookup(&c, "artist");

	info->title[4] = "Copyright: ";
	info->info[4] = corlett_tag_lookup(&c, "copyright");

	info->title[5] = "Year: ";
	info->info[5] = corlett_tag_lookup(&c, "year");

	info->title[6] = "Length: ";
	info->info[6] = corlett_tag_lookup(&c, "length");

	info->title[7] = "Fade: ";
	info->info[7] = corlett_tag_lookup(&c, "fade");

	return AO_SUCCESS;
}

int32 am2snd_fill_info(ao_display_info *info)
{
    info->title[1] = "Name: ";
    info->info[1] = "Unknown";

    info->title[2] = "Game: ";
    info->info[2] = "Unknown";

    info->title[3] = "Artist: ";
    info->info[3] = "Unknown";

    info->title[4] = "Copyright: ";
    info->info[4] = "Unknown";

    info->title[5] = "Year: ";
    info->info[5] = "Unknown";

    info->title[6] = "Length: ";
    info->info[6] = "Unknown";

    info->title[7] = "Fade: ";
    info->info[7] = "Unknown";

    return AO_SUCCESS;
}

