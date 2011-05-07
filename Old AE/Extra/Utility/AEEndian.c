#include "AEEndian.h"

#if defined(AEEndianLittle)
static inline uint16_t FlipU16(uint16_t value){
	return ((value >> 8) & 0x00ff) | ((value << 8) & 0xff00));
}

static inline uint32_t FlipU32(uint32_t value){
	return ((value >>24) & 0x000000ff) | ((value >> 8) & 0x0000ff00) | ((value << 8) & 0x00ff0000) | ((value <<24) & 0xff000000);
}

//From Carbon/Endian.h <APSL 2.0>
static inline uint64_t FlipU64(uint64_t value){
	return (((((uint64_t)value)<<56) & 0xFF00000000000000ULL)  | \
                 ((((uint64_t)value)<<40) & 0x00FF000000000000ULL)  | \
                 ((((uint64_t)value)<<24) & 0x0000FF0000000000ULL)  | \
                 ((((uint64_t)value)<< 8) & 0x000000FF00000000ULL)  | \
                 ((((uint64_t)value)>> 8) & 0x00000000FF000000ULL)  | \
                 ((((uint64_t)value)>>24) & 0x0000000000FF0000ULL)  | \
                 ((((uint64_t)value)>>40) & 0x000000000000FF00ULL)  | \
                 ((((uint64_t)value)>>56) & 0x00000000000000FFULL))
}
//</APSL 2.0>
#define HostFromNetU16(i) FlipU16(i)
#define HostFromNetU32(i) FlipU32(i)
#define HostFromNetU64(i) FlipU64(i)
#define NetFromHostU16(i) FlipU16(i)
#define NetFromHostU32(i) FlipU32(i)
#define NetFromHostU64(i) FlipU64(i)

#elif defined(AEEndianBig)
#define HostFromNetU16(i) (i)
#define HostFromNetU32(i) (i)
#define HostFromNetU64(i) (i)
#define NetFromHostU16(i) (i)
#define NetFromHostU32(i) (i)
#define NetFromHostU64(i) (i)

#else

static inline uint64_t TwoToThe(int exponent){
	uint64_t x=1;
	for(int i=0;i<exponent;i++) x*=2;
	return x;
}

static inline uint64_t FlipFrom(uint8_t* bytes, bool bigendian, int size){
	uint64_t integer=0;
	for(int i=0;i<size;i++){
		int byteID=i;
		if(bigendian) byteID=size-i-1;
		integer+=(uint64_t)bytes[byteID]*TwoToThe(size*i);
	}
	return integer;
}

static inline void FlipTo(uint64_t in, uint8_t* out, bool bigendian, int size){
	uint64_t quotient=in;
	for(int i=0;i<size;i++) out[i]=0;
	for(int i=0;i<size;i++){
		int byteID=i;
		if(bigendian) byteID=size-i-1;
		if(quotient==0) break;
		out[byteID]=quotient%256;
		quotient/=256;
	}
}

static inline uint64_t HostFromNetU64(uint64_t netu64){
	return FlipFrom((uint8_t*)&netu64, true, 8);
}

static inline uint64_t NetFromHostU64(uint64_t hostu64){
	FlipTo(hostu64, (uint8_t*)&hostu64, true, 8);
	return hostu64;
}

static inline uint32_t HostFromNetU32(uint32_t netu32){
	return FlipFrom((uint8_t*)&netu32, true, 4);
}

static inline uint32_t NetFromHostU32(uint32_t hostu32){
	FlipTo(hostu32, (uint8_t*)&hostu32, true, 4);
	return hostu32;
}

static inline uint16_t HostFromNetU16(uint16_t netu16){
	return FlipFrom((uint8_t*)&netu16, true, 4);
}

static inline uint16_t NetFromHostU16(uint16_t hostu16){
	FlipTo(hostu16, (uint8_t*)&hostu16, true, 4);
	return hostu16;
}

#endif

void AEHostFromNet64(void* host, const void* net, size_t count){
	uint64_t* host64s=host;
	const uint64_t* net64s=net;
	for (size_t i=0; i<count; i++) 
		host64s[i]=HostFromNetU64(net64s[i]);
}

void AENetFromHost64(void* net, const void* host, size_t count){
	const uint64_t* host64s=host;
	uint64_t* net64s=net;
	for (size_t i=0; i<count; i++) 
		net64s[i]=NetFromHostU64(host64s[i]);
}

void AEHostFromNet32(void* host, const void* net, size_t count){
	uint32_t* host32s=host;
	const uint32_t* net32s=net;
	for (size_t i=0; i<count; i++) 
		host32s[i]=HostFromNetU32(net32s[i]);
}

void AENetFromHost32(void* net, const void* host, size_t count){
	const uint32_t* host32s=host;
	uint32_t* net32s=net;
	for (size_t i=0; i<count; i++) 
		net32s[i]=NetFromHostU32(host32s[i]);
}

void AEHostFromNet16(void* host, const void* net, size_t count){
	uint16_t* host16s=host;
	const uint16_t* net16s=net;
	for (size_t i=0; i<count; i++) 
		host16s[i]=HostFromNetU16(net16s[i]);
}

void AENetFromHost16(void* net, const void* host, size_t count){
	const uint16_t* host16s=host;
	uint16_t* net16s=net;
	for (size_t i=0; i<count; i++) 
		net16s[i]=NetFromHostU16(host16s[i]);
}
