/*
 * timecheck.h
 *
 *  Created on: 27 Jun 2024
 *      Author: Maldus
 */

#ifndef SERVICES_TIMESTAMP_H_
#define SERVICES_TIMESTAMP_H_

#include <stdint.h>


#define TIME_AFTER(a, b)          (((long)((b) - (a)) < 0))
#define TIME_AFTER_OR_EQUAL(a, b) (((long)((b) - (a)) <= 0))


static inline __attribute__((always_inline)) uint8_t timestamp_is_expired(unsigned long start, unsigned long current,
                                                                unsigned long duration) {
    return TIME_AFTER_OR_EQUAL(current, start + duration);
}


static inline __attribute__((always_inline)) unsigned long timestamp_difference(unsigned long a, unsigned long b) {
    if (TIME_AFTER(a, b))
        return -((unsigned long)b - (unsigned long)a);
    else
        return (unsigned long)b - (unsigned long)a;
}


#endif /* SERVICES_TIMESTAMP_H_ */
