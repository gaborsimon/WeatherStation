#ifndef UTILS_H_
#define UTILS_H_


#define BIT_SET(reg,bit)    ((reg) |=  (1u << (bit)))
#define BIT_CLR(reg,bit)    ((reg) &= ~(1u << (bit)))
#define BIT_GET(reg,bit)    ((reg)  &  (1u << (bit)))

#define DISABLE_INTERRUPT() __asm__ __volatile__("cli")
#define ENABLE_INTERRUPT()  __asm__ __volatile__("sei")
#define WD_RESET()          __asm__ __volatile__("wdr")

#define INIT_VALUE_FLOAT (0.0f)
#define INIT_VALUE_UINT    (0u)
#define INIT_VALUE_SINT    (0)

#define TRUE    (1u)
#define FALSE   (0u)

#define INPUT   (0u)
#define OUTPUT  (1u)

#define LOW     (0u)
#define HIGH    (1u)

#define PI          ((float32) (3.141592654f))

#define DEG_TO_RAD(degree)      ((float32)degree * (PI / 180.0f))
#define RAD_TO_DEG(radian)      ((float32)radian * (180.0f / PI))

#define SIND(degree)            (sin(DEG_TO_RAD(degree)))
#define COSD(degree)            (cos(DEG_TO_RAD(degree)))
#define NORMALIZE(raw_degree)   (raw_degree - (floor(raw_degree / 360.0f) * 360.0f))


#endif /* UTILS_H_ */
