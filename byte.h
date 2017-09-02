
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || \
    defined(__BIG_ENDIAN__) || \
    defined(__ARMEB__) || \
    defined(__THUMBEB__) || \
    defined(__AARCH64EB__) || \
    defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
#define HIBYTE(w)   ((uint8_t)(w))
#define LOBYTE(w)   ((uint8_t)(((uint16_t)(w) >> (uint8_t)8) & 0xff))
#else
#define LOBYTE(w)   ((uint8_t)(w))
#define HIBYTE(w)   ((uint8_t)(((uint16_t)(w) >> (uint8_t)8) & 0xff))
#endif
