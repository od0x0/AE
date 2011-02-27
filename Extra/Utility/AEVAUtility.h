#pragma once
#include "AE.h"

void AEVASerializeToMBuffer(AEVA* self,AEMBuffer* mbuffer);
void AEVAUnserializeFromMBuffer(AEVA* self,AEMBuffer* mbuffer);

//A little note about serialization...  By the nature of serialization, the data that is serialized is highly unportable.  While it is possible to make serialized index arrays cross platform (which deal with integers), it is another matter for vertex arrays (which deal with 32bit floats), as the C standard makes no real guarrantee about the format of a floating point number.  As such, in order for this to be used properly, you should serialize the vertex array for every possible architecture you plan to run on.

void AEVALoadFromObj(AEVA* va, AEVA* ia, const char* objfilename);

//A few notes about loading from .obj's, 1) It will only pickup triangulated meshes, there are just too many variants to deal with for it to bother with triangulating your stuff, you should be manually triangulating anyway.  2) Leaving the "ia" argument as NULL will generate va as ready for glDrawArrays() 3) This loader will only pickup normals, positions, and texture coordinates, if you want more, write your own loader.