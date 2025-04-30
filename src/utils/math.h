#ifndef _UTIL_MATH_H
#define _UTIL_MATH_H

typedef struct { int x, y; } int2_t;
typedef struct { int x, y, z; } int3_t;
typedef struct { int x, y, z, w; } int4_t;

typedef struct { unsigned int x, y; } uint2_t;
typedef struct { unsigned int x, y, z; } uint3_t;
typedef struct { unsigned int x, y, z, w; } uint4_t;

typedef struct { float x, y; } float2_t;
typedef struct { float x, y, z; } float3_t;
typedef struct { float x, y, z, w; } float4_t;

typedef struct { double x, y; } double2_t;
typedef struct { double x, y, z; } double3_t;
typedef struct { double x, y, z, w; } double4_t;

#define int2(x, y) (int2_t){x, y}
#define int3(x, y, z) (int3_t) {x, y, z}
#define int4(x, y, z, w) (int4_t) {x, y, z, w}

#define uint2(x, y) (uint2_t) {x, y}
#define uint3(x, y, z) (uint3_t) {x, y, z}
#define uint4(x, y, z, w) (uint4_t) {x, y, z, w}

#define float2(x, y) (float2_t){x, y}
#define float3(x, y, z) (float3_t) {x, y, z}
#define float4(x, y, z, w) (float4_t) {x, y, z, w}

#define double2(x, y) (double2_t){x, y}
#define double3(x, y, z) (double3_t) {x, y, z}
#define double4(x, y, z, w) (double4_t) {x, y, z, w}

typedef unsigned int uint;

#endif //_UTIL_MATH_H
