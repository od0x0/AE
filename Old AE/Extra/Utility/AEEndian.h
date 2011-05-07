#pragma once
#include "AE.h"

//#define AEEndianLittle
//#define AEEndianBig
void AEHostFromNet64(void* host, const void* net, size_t count);
void AENetFromHost64(void* net, const void* host, size_t count);
void AEHostFromNet32(void* host, const void* net, size_t count);
void AENetFromHost32(void* net, const void* host, size_t count);
void AEHostFromNet16(void* host, const void* net, size_t count);
void AENetFromHost16(void* net, const void* host, size_t count);
