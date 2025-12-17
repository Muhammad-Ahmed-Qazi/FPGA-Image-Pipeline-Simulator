// In fixed_point.h:
#ifdef USE_FIXED_POINT
    typedef int Fixed;
    #define FP_SCALE 256
    #define TO_FIXED(x) ((Fixed)((x) * FP_SCALE))
    #define FROM_FIXED(x) ((uint8_t)((x) / FP_SCALE))
#else
    typedef float Fixed;
    #define TO_FIXED(x) (x)
    #define FROM_FIXED(x) ((uint8_t)(x))
#endif