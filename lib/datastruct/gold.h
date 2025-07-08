#ifdef __cplusplus
extern "C" {
#endif

int count_one32(unsigned int in);
void gold_gen(unsigned short _x1,
              unsigned short _x2,
              unsigned short _n1,
              unsigned short _n2,
              unsigned char* gold);

void gold_buf_gen(unsigned char* gold, unsigned int* gold_buf);

#ifdef __cplusplus
}
#endif
