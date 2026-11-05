#ifndef GFX_BITS_HLSLI
#define GFX_BITS_HLSLI

#define gfxBitMask(width) ((width) == 32u ? 0xffffffffu : ((1u << (width)) - 1u))
#define gfxInsertMask(offset, width) ((width) == 32u ? 0xffffffffu : (gfxBitMask(width) << ((offset) & 31u)))
#define gfxInsertBits(base, value, offset, width) (((base) & ~gfxInsertMask(offset, width)) | (((value) << (offset)) & gfxInsertMask(offset, width)))
#define gfxExtractBits(value, offset, width) (((value) >> (offset)) & gfxBitMask(width))
#define gfxExtractSignedBits(value, offset, width) ((((value) >> (offset)) << ((32 - (width)) & 31)) >> ((32 - (width)) & 31))

float2 gfxUnpackHalf2(uint value) {
    return f16tof32(uint2(value & 0xffffu, value >> 16));
}

#endif
