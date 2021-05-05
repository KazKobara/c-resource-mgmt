/*
 How to compile and run.
 $ gcc -o struct struct.c
 $ ./struct
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#define I64_LEN 2  // > 1
#define STR_LEN 5

/**
 * @brief Print properties of a sample struct whose type is any of
 *        struct___aligned___packed_untruncated_t,
 *        struct___aligned___packed___truncated_t, and 
 *        struct___aligned___packed___truncated_with_pragma_t.
 * @note The column of 'pad' shows the padding size in octet to align.
 *       Padding might contain garbage.
 * @note The addresses of bit-field 'b2' are not addressable and
 *       shown as '0x??...'. The 'pad' size after the bit-field is summed
 *       up with that of the previous member.
 * @note The column of 'al' shows the shift in octets from the appropriate
 *       alignment position for multiple-octet members.
 *       For single-octet members, `-` are shown since they are free from
 *       alignment.
 */
 /* NOTE:
 *  This macro can be used commonly among the above struct-types,
 *  whereas functions have to be prepared for each struct type
 *  since each struct type has its address map on its members.
 */
#define print_struct(x) \
do { \
    (void)printf("struct.member : address      size pad al value\n"); \
    (void)printf("--------------:-----------------------------\n"); \
    (void)printf("struct        : %p %2zu\n", &(x), sizeof((x))); \
    (void)printf("struct.f128   : %p %2zu %3lu %2lu %Lf\n", \
        &((x).f128), sizeof((x).f128), \
        (uintptr_t)&((x).i64)-(uintptr_t)&((x).f128)-sizeof((x).f128), \
        (((uintptr_t)&((x).f128)) & (sizeof(long double)-1)), (x).f128); \
    (void)printf("struct.i64[%d] : %p %2zu %3lu %2lu {%" PRId64 ", %" PRId64 "}\n", I64_LEN, \
        &((x).i64), sizeof((x).i64), (uintptr_t)&((x).f64)-(uintptr_t)&((x).i64)-sizeof((x).i64), \
        (((uintptr_t)&((x).i64)) & (sizeof(int64_t)-1)), (x).i64[0], (x).i64[1]); \
    (void)printf("struct.f64    : %p %2zu %3lu %2lu %f \n", \
        &((x).f64), sizeof((x).f64), (uintptr_t)&((x).i32)-(uintptr_t)&((x).f64)-sizeof((x).f64), \
        (((uintptr_t)&((x).f64)) & (sizeof(double)-1)), (x).f64); \
    (void)printf("struct.i32    : %p %2zu %3lu %2lu %d \n", \
        &((x).i32), sizeof((x).i32), (uintptr_t)&((x).f32)-(uintptr_t)&((x).i32)-sizeof((x).i32), \
        (((uintptr_t)&((x).i32)) & (sizeof(int32_t)-1)), (x).i32); \
    (void)printf("struct.f32    : %p %2zu %3lu %2lu %f \n", \
        &((x).f32), sizeof((x).f32), (uintptr_t)&((x).i16)-(uintptr_t)&((x).f32)-sizeof((x).f32), \
        (((uintptr_t)&((x).f32)) & (sizeof(float)-1)), (x).f32); \
    (void)printf("struct.i16    : %p %2zu %3lu %2lu %d \n", \
        &((x).i16), sizeof((x).i16), (uintptr_t)&((x).i8)-(uintptr_t)&((x).i16)-sizeof((x).i16), \
        (((uintptr_t)&((x).i16)) & (sizeof(int16_t)-1)), (x).i16); \
    (void)printf("struct.i8     : %p %2zu   0  - %d \n", \
        &((x).i8), sizeof((x).i8), \
        (x).i8); \
    (void)printf("struct.b2     : 0x%s  1 %3lu  - %d\n", \
        sizeof(uintptr_t)>4 ? "????????????" : "????????",\
        (uintptr_t)&((x).str)-(uintptr_t)&((x).i8)-sizeof((x).i8)-1, (x).b2); \
    (void)printf("struct.str[%d] : %p %2zu %3lu  - \"%s\" \n", STR_LEN, \
        &((x).str), sizeof((x).str), sizeof((x))-((uintptr_t)&((x).str)-(uintptr_t)&(x))-sizeof((x).str), \
        (x).str); \
    (void)printf("\n"); \
} while(0)

/**
 * @brief Print properties of a sample struct whose type is any of
 *        struct___aligned_unpacked_untruncated_t,
 *        struct_unaligned___packed___truncated_t, and
 *        struct_unaligned___packed___truncated_with_pragma_t.
 * @note The column of 'pad' shows the padding size in octet to align.
 *       Padding might contain garbage.
 * @note The addresses of bit-field 'b2' are not addressable and
 *       shown as '0x??...'. The 'pad' size after the bit-field is summed
 *       up with that of the previous member.
 * @note The column of 'al' shows the shift in octets from the appropriate
 *       alignment position for multiple-octet members.
 *       For single-octet members, `-` are shown since they are free from
 *       alignment.
 */
 /* NOTE:
 *  This macro can be used commonly among the above struct-types,
 *  whereas functions have to be prepared for each struct type
 *  since each struct type has its address map on its members.
 */
#define print_messy_struct(x) \
do { \
    (void)printf("struct.member : address      size pad al value\n"); \
    (void)printf("--------------:-----------------------------\n"); \
    (void)printf("struct        : %p %2zu\n", &(x), sizeof((x))); \
    (void)printf("struct.b2     : 0x%s  1 %3lu  - %d\n", \
        sizeof(uintptr_t)>4 ? "????????????" : "????????", \
        (uintptr_t)&((x).f128)-(uintptr_t)&(x)-1, (x).b2); \
    (void)printf("struct.f128   : %p %2zu %3lu %2lu %Lf\n", \
        &((x).f128), sizeof((x).f128), (uintptr_t)&((x).i8)-(uintptr_t)&((x).f128)-sizeof((x).f128), \
        (((uintptr_t)&((x).f128)) & (sizeof(long double)-1)), (x).f128); \
    (void)printf("struct.i8     : %p %2zu %3lu  - %d \n", \
        &((x).i8), sizeof((x).i8), (uintptr_t)&((x).i64)-(uintptr_t)&((x).i8)-sizeof((x).i8), (x).i8); \
    (void)printf("struct.i64[%d] : %p %2zu %3lu %2lu {%" PRId64 ", %" PRId64 "}\n", I64_LEN, \
        &((x).i64), sizeof((x).i64), (uintptr_t)&((x).i16)-(uintptr_t)&((x).i64)-sizeof((x).i64), \
        (((uintptr_t)&((x).i64)) & (sizeof(int64_t)-1)), (x).i64[0], (x).i64[1]); \
    (void)printf("struct.i16    : %p %2zu %3lu %2lu %d \n", \
        &((x).i16), sizeof((x).i16), (uintptr_t)&((x).f64)-(uintptr_t)&((x).i16)-sizeof((x).i16), \
        (((uintptr_t)&((x).i16)) & (sizeof(int16_t)-1)), (x).i16); \
    (void)printf("struct.f64    : %p %2zu %3lu %2lu %f \n", \
        &((x).f64), sizeof((x).f64), (uintptr_t)&((x).i32)-(uintptr_t)&((x).f64)-sizeof((x).f64), \
        (((uintptr_t)&((x).f64)) & (sizeof(double)-1)), (x).f64); \
    (void)printf("struct.i32    : %p %2zu %3lu %2lu %d \n", \
        &((x).i32), sizeof((x).i32), (uintptr_t)&((x).str)-(uintptr_t)&((x).i32)-sizeof((x).i32), \
        (((uintptr_t)&((x).i32)) & (sizeof(int32_t)-1)), (x).i32); \
    (void)printf("struct.str[%d] : %p %2zu %3lu  - \"%s\" \n", STR_LEN, \
        &((x).str),  sizeof((x).str), (uintptr_t)&((x).f32)-(uintptr_t)&((x).str)-sizeof((x).str), (x).str); \
    (void)printf("struct.f32    : %p %2zu %3lu %2lu %f \n", \
        &((x).f32), sizeof((x).f32) ,sizeof((x))-((uintptr_t)&((x).f32)-(uintptr_t)&(x))-sizeof((x).f32), \
        (((uintptr_t)&((x).f32)) & (sizeof(float)-1)), (x).f32); \
    (void)printf("\n"); \
} while(0)

/*** Various structs with the same members ***/
/* Members in a struct should be placed so that they are packed with
 no padding and aligned. */

/** Bad examples!! **/
/* They include padding or/and are not aligned. */
typedef struct {            // member_size = type_size * blocks [octet]
    uint8_t b2:2;           // 1 (6 bit padding + 2 bits)
                            // 15 octet padding
    long double f128;       // 16
    uint8_t i8;             // 1
                            // 7 octet padding
    int64_t i64[I64_LEN];   // 8*2
    int16_t i16;            // 2
                            // 6 octet padding
    double  f64;            // 8
    int32_t i32;            // 4
    char    str[STR_LEN];   // 1*5
                            // 7 octet padding
    float   f32;            // 4
                            // 15 octet padding
                            // 96 in total
} struct___aligned_unpacked_untruncated_t ;

typedef struct {            // member_size = type_size * blocks [octet]
    uint8_t b2:2;           // 1 (6 bit padding + 2 bits)
    long double f128;       // 16
    uint8_t i8;             // 1
    int64_t i64[I64_LEN];   // 8*2
    int16_t i16;            // 2
    double  f64;            // 8
    int32_t i32;            // 4
    char    str[STR_LEN];   // 1*5
    float   f32;            // 4
                            // 57 in total
} __attribute__((__packed__)) struct_unaligned___packed___truncated_t ;

#pragma pack(push, 1)
typedef struct {            // member_size = type_size * blocks [octet]
    uint8_t b2:2;           // 1 (6 bit padding + 2 bits)
    long double f128;       // 16
    uint8_t i8;             // 1
    int64_t i64[I64_LEN];   // 8*2
    int16_t i16;            // 2
    double  f64;            // 8
    int32_t i32;            // 4
    char    str[STR_LEN];   // 1*5
    float   f32;            // 4
                            // 57 in total
} struct_unaligned___packed___truncated_with_pragma_t ;
#pragma pack(pop)

/* Still not a good example */
typedef struct {           // member_size = type_size * blocks [octet]
    long double f128;      // 16
    int64_t i64[I64_LEN];  // 8*2
    double  f64;           // 8
    int32_t i32;           // 4
    float   f32;           // 4
    int16_t i16;           // 2
    uint8_t i8;            // 1
    uint8_t b2:2;          // 1 (6 bits padding + 2 bits)
    char    str[STR_LEN];  // 1*5
                           // 7 octet padding
                           // 64 in total
} struct___aligned___packed_untruncated_t;

/*** Both packed and aligned ***/
typedef struct {            // member_size = type_size * blocks [octet]
    long double f128;       // 16
    int64_t i64[I64_LEN];   // 8*2
    double  f64;            // 8
    int32_t i32;            // 4
    float   f32;            // 4
    int16_t i16;            // 2
    uint8_t i8;             // 1
    uint8_t b2:2;           // 1 (6 bits padding + 2 bits)
    char    str[STR_LEN];   // 1*5
                            // 57 in total
} __attribute__((__packed__)) struct___aligned___packed___truncated_t;

#pragma pack(push, 1)
typedef struct {            // member_size = type_size * blocks [octet]
    long double f128;       // 16
    int64_t i64[I64_LEN];   // 8*2
    double  f64;            // 8
    int32_t i32;            // 4
    float   f32;            // 4
    int16_t i16;            // 2
    uint8_t i8;             // 1
    uint8_t b2:2;           // 1 (6 bits padding + 2 bits)
    char    str[STR_LEN];   // 1*5
                            // 57 in total
} struct___aligned___packed___truncated_with_pragma_t;
#pragma pack(pop)

/**
 * @brief Dump *buf of len octets
 * 
 * @param buf 
 * @param len 
 * @return int32_t 
 */
#define LINE_WIDTH 16
uint32_t buf_dump(const unsigned char * const buf, const uint32_t len)
{
    uint32_t i = 0, tmp;
    char str_buf[LINE_WIDTH + 1];

    memset(str_buf,0,LINE_WIDTH + 1);
    for (i = 0; i < len; i++){
        (void)printf("%02X ", buf[i]);
        str_buf[i % LINE_WIDTH] = ( 32 <= buf[i] && buf[i] <= 126 ) ? buf[i] : '.';
        if ( i % LINE_WIDTH == LINE_WIDTH - 1 ){
        (void)printf("  %s\n", str_buf);
        }
    }

    if ( (tmp = i % LINE_WIDTH) != 0 ){
        for (i = 0; i < LINE_WIDTH - tmp; i++) printf("   ");
        str_buf[tmp] = 0;
        (void)printf("  %s\n", str_buf);
    }
    /*  (void)printf("\n"); */

    return i;
}

void print_struct_size_and_addr(void)
{
    struct___aligned_unpacked_untruncated_t struct___aligned_unpacked_untruncated;
    struct_unaligned___packed___truncated_t struct_unaligned___packed___truncated;
    struct_unaligned___packed___truncated_with_pragma_t struct_unaligned___packed___truncated_with_pragma;
    struct___aligned___packed_untruncated_t struct___aligned___packed_untruncated;
    struct___aligned___packed___truncated_t struct___aligned___packed___truncated;
    struct___aligned___packed___truncated_with_pragma_t struct___aligned___packed___truncated_with_pragma;

    /* Structs here are neither initialized nor zero-cleared intentionally
     * to show the garbage in them. */

    (void)printf("\n============== Size of Various Structs ==============\n");
    (void)printf("\n");
    (void)printf("              Type of struct                        : Size [octet]\n");
    (void)printf("---------------------------------------------------------------------\n");
    (void)printf("struct___aligned_unpacked_untruncated_t             : %lu\n",
                    sizeof(struct___aligned_unpacked_untruncated));
    (void)printf("struct_unaligned___packed___truncated_t             : %lu\n",
                    sizeof(struct_unaligned___packed___truncated));
    (void)printf("struct_unaligned___packed___truncated_with_pragma_t : %lu\n",
                    sizeof(struct_unaligned___packed___truncated_with_pragma));
    (void)printf("struct___aligned___packed_untruncated_t             : %lu\n",
                    sizeof(struct___aligned___packed_untruncated));
    (void)printf("struct___aligned___packed___truncated_t             : %lu\n",
                    sizeof(struct___aligned___packed___truncated));
    (void)printf("struct___aligned___packed___truncated_with_pragma_t : %lu\n",
                    sizeof(struct___aligned___packed___truncated_with_pragma));
    (void)printf("\n");

    (void)printf("\n====================== Details ======================\n\n");
    
    (void)printf("------- struct___aligned_unpacked_untruncated_t -------\n");
    print_messy_struct(struct___aligned_unpacked_untruncated);
    (void)printf("------- struct_unaligned___packed___truncated_t -------\n");
    print_messy_struct(struct_unaligned___packed___truncated);
    (void)printf("------- struct_unaligned___packed___truncated_with_pragma_t -------\n");
    print_messy_struct(struct_unaligned___packed___truncated_with_pragma);

    (void)printf("------- struct___aligned___packed_untruncated_t -------\n");
    print_struct(struct___aligned___packed_untruncated);
    (void)printf("------- struct___aligned___packed___truncated_t -------\n");
    print_struct(struct___aligned___packed___truncated);
    (void)printf("------- struct___aligned___packed___truncated_with_pragma_t -------\n");
    print_struct(struct___aligned___packed___truncated_with_pragma);

}

void garbage_in_struct(void)
{
    struct___aligned_unpacked_untruncated_t dest_struct, org_struct = {1,16.0,1,{8,8},2,8.0,4,"char",4.0};

    (void)printf("%p    org_struct\n\n", &org_struct);
    print_messy_struct(org_struct);
    (void)buf_dump((const unsigned char*) &org_struct,  sizeof(org_struct));
    (void)printf("\n");

    (void)printf("%p    dest_struct\n", &dest_struct);
    (void)buf_dump((const unsigned char*) &dest_struct, sizeof(dest_struct));
    (void)printf("\n");

    memset(&dest_struct, 0, sizeof(dest_struct));
    (void)printf("%p    dest_struct after zero-cleared\n", &dest_struct);
    (void)buf_dump((const unsigned char*) &dest_struct, sizeof(dest_struct));
    (void)printf("\n");

    /* Bad example: garbage of org_struct will be copied!! */
    dest_struct = org_struct;
    (void)printf("%p    dest_struct = org_struct\n", &dest_struct);
    (void)buf_dump((const unsigned char*) &dest_struct, sizeof(dest_struct));
    (void)printf("\n");
}


int32_t main(void)
{
    (void)printf("\n==================== Garbage in Struct =====================\n\n");
    garbage_in_struct();

    (void)printf("\n========= Padding and Alignment in Various Structs =========\n");
    print_struct_size_and_addr();

    return 0;
}