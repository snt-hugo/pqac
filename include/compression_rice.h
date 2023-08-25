#ifndef COMPRESSION_RICE_H
#define COMPRESSION_RICE_H

#include "new_arithm_poly.h"
#include "params.h"

#define CEIL_LOG_S 85
#define CEIL_LOG_R 77
#define CEIL_LOG XI 20
#define CEIL_LOG_XI1 104 
#define CEIL_LOG_XI2 113 

unsigned long long compression_rice_intq1(uint8_t *initial_value, intq1_t sampled_integer_input,
                                          int parameter, int position);

unsigned long long decompression_rice_intq1(intq1_t integer, uint8_t *compressed_value,
                                            int parameter, int position);

unsigned long long compression_rice_polyq1(uint8_t *compression_space, polyq1_t poly_input,
                                           int parameter, int position);

unsigned long long decompression_rice_polyq1(polyq1_t polyq1_output, uint8_t *compression_space,
                                             int parameter, int position);

unsigned long long compression_rice_intq2(uint8_t *initial_value, intq2_t sampled_integer_input,
                                          int parameter, int position);

unsigned long long decompression_rice_intq2(intq2_t integer, uint8_t *compressed_value,
                                            int parameter, int position);

unsigned long long compression_rice_polyq2(uint8_t *compression_space, polyq2_t poly_input,
                                           int parameter, int position);

unsigned long long decompression_rice_polyq2(polyq2_t polyq2_output, uint8_t *compression_space,
                                             int parameter, int position);

unsigned long long compression_rice_intQ(uint8_t *initial_value, intQ_t sampled_integer_input,
                                          int parameter, int position);

unsigned long long decompression_rice_intQ(intQ_t integer, uint8_t *compressed_value,
                                            int parameter, int position);

unsigned long long compression_rice_polyQ(uint8_t *compression_space, polyQ_t poly_input,
                                           int parameter, int position);

unsigned long long decompression_rice_polyQ(polyQ_t polyQ_output, uint8_t *compression_space,
                                             int parameter, int position);

#endif /* COMPRESSION_RICE_H */
