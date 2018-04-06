#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../include/rs.h"
#include "../../inlcude/lrc.h"
//#include "utils.h"
//#include "common.h"

void *rs_encode(struct Encoder *encoder, void **args) {
    // args[0] = local size *
    // args[1] = local int *
    // args[2] = global size *
    // args[3] = buffer size *
    struct timeval encoding_time_lrc;
//    printf("already entered the proccedure of encoding!!!\n");

    int size = *((int *)(args[3]));
    int local_size = *((int *)(args[0]));
    int global_size = *((int *)(args[2]));
    uint8_t *local_arr = (uint8_t *)(args[1]);
    int local_size_total = 0;
    for (int i = 0; i < local_size; i ++)
        local_size_total += local_arr[i];

    // printf("encode %d %d %d\n", size, local_size, global_size + local_size_total);
    // for (int i = 0; i < local_size; i ++)
    //     printf(" %d ", local_arr[i]);
    // printf("\n");
//    printf("already finshed  the proccedure of parameter configure!!!\n");

    lrc_t     *lrc  = &(lrc_t) {0};
    lrc_buf_t *buf  = MALLOC(lrc_buf_t, 1);
    buf->inited_ = 0;
    lrc->inited_ = 0;
    // &(lrc_buf_t) {0};
    if (lrc_init_n(lrc, local_size, local_arr, global_size + local_size) != 0) {
        // lrc_destroy(lrc);
        // lrc_buf_destroy(buf);
        printf("encoder rs init error0 %d\n", lrc_init_n(lrc, local_size, local_arr, global_size + local_size));
        return NULL;
    }

////////////////////////////////////
//for debugging
/*    
    printf("**********local_size_total is %d************\n", local_size_total);
    printf("**********global_size is %d************\n", global_size);
    printf("**********lrc_n is %d************\n", lrc->n);
*/
////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
//modified by shawnshanks_fei, date:20170915.pm
/*    
    lrc->k = DATA_LENTH * 4 / WIDTH;
    lrc->m = GLOBAL_SIZE + 1;
    lrc->n = lrc->k + lrc->m;
*/
/////////////////////////////////////////////////////////////////////////////////////////
//    printf("**********lrc_n is %d************\n", lrc->n);

    if (lrc_buf_init(buf, lrc, size) != 0) {
        // printf("encoder rs init error1\n");
        lrc_destroy(lrc);
        free(buf);
        return NULL;
    }

//    printf("already finshed the inition of encoding!!!\n");

    // printf("malloc\n");
    struct MyRSPair *ret = NULL;
    ret = MALLOC(struct MyRSPair, (local_size + global_size + local_size_total));
    for (int i = 0; i < global_size + local_size_total + local_size; i ++) {
        ret[i].id = 0;
        ret[i].symbol = NULL;
    }
    // printf("addr = %lld\n", ret);
    // free(ret); return NULL;
    // return ret;
    // #ifdef TEST
//    printf("hello,world-1!!!\n");    

    // printf("%d\n",)
    int indexs = 0;
    for (int i = 0; i < encoder->dpdk_data_maker->data_len * 4; i += size, indexs ++) {
        int j = i + size, k = j;
        if (j > encoder->dpdk_data_maker->data_len * 4)
            j = encoder->dpdk_data_maker->data_len * 4;
//        printf("%d %d\n", i, j);
        memcpy(buf->data[indexs], ((char *)(encoder->dpdk_data_maker->data)) + i, j - i);   
    }

//    printf("hello,world!!!\n");
    if (lrc_encode(lrc, buf) != 0) {
        // printf("rs encode fail\n");
        lrc_destroy(lrc);
        lrc_buf_destroy(buf);
        free(buf);
        free(ret);
        return NULL;
    }
    
///////////////////////////////////////////////////////////////////////////
//function: free the memory of data_maker proccedure!!!!!!!
//added by shawnshanks_fei, date:20170914.pm
//    free((uint32_t *)encoder->dpdk_data_maker->data);
///////////////////////////////////////////////////////////////////////////

//    printf("already finshed the procedure of encoding in rs.c!!!\n");
    
    // // char *global_buffer = MALLOC(char, (size + 1) * global_size);
    // printf("%d\n",  global_size + local_size_total);
    // srand((unsigned int)time(NULL)); 
    
    int *cntsss = NULL;
    cntsss = malloc(sizeof(int));
    encoder->transmit->start(encoder->transmit, local_size_total + global_size + local_size, encoder->dpdk_data_maker->drop_prob);
    indexs = 0;
    for (int i = 0; i < local_size_total + global_size; i ++) {
        float dropped = ((float)(rand()) / (float) RAND_MAX) * (float)100.0; //original
 //       float dropped = 10;   //modified
        if (encoder->transmit->drop(encoder->transmit, i, dropped, encoder->dpdk_data_maker->drop_prob) == 0) {
            char *dest;
            if (i < local_size_total)
                dest = buf->data[i];
            else
                dest = buf->code[i - local_size_total];
            // printf("%lld %lld\n", &(ret[indexs].symbol), &(ret[indexs].id));
            // modify
            // ret[indexs].symbol = dest;
            ret[indexs].symbol = MALLOC(char, size);
            memcpy(ret[indexs].symbol, dest, size);
            // printf("dest = %lld %lld\n", dest, ret[indexs].symbol);
            ret[indexs].id = i;
            // printf("%d %d %d\n", indexs, ret[3].id, i);
            indexs = indexs + 1;
        }
    }


    *cntsss = indexs;
    // printf("cntsss = %d %lld %lld\n", *cntsss, cntsss, &(ret[3].id));
    // for (int i = 0; i < indexs; i ++) {
    //     printf("snd %d %d %i\n", i, ret[3].id, ret[i].id);
    // }

    // printf("origin data\n");
    // for (int i = 0; i < local_size_total; i ++) {
    //     for (int j = 0; j < size; j ++)
    //         printf("%03d ", *(buf->data[i] + j));
    //     printf("\n");
    // }
    // printf("encode data\n");
    // for (int i = 0; i < global_size; i ++) {
    //     for (int j = 0; j < size; j ++)
    //         printf("%03d ", *(buf->code[i] + j));
    //     printf("\n");
    // }
    // for (int i = 0; i < indexs; i ++) {
    //     printf("snd %d\n", ret[i].id);
    // }
    void **result = NULL;
    result = MALLOC(void *, 3);
    result[0] = (void *)ret;
    // // free(ret);
    // // printf("%lld %lld\n", result[0], ret);
    // // free(ret);
    result[1] = (void *)cntsss;
    // result[2] = (void *)buf;

    lrc_buf_destroy(buf);
    free(buf);
    // free(buf);
    lrc_destroy(lrc);
    // free(ret);
    // printf("encode finish\n");
    // return result;
    return result;
}

void *rs_decode(struct Decoder *decoder, void **args) {
    int size = *((int *)(args[3]));
    int local_size = *((int *)(args[0]));
    int global_size = *((int *)(args[2]));
    uint8_t *local_arr = (uint8_t *)(args[1]);
    int local_size_total = 0;
    for (int i = 0; i < local_size; i ++)
        local_size_total += local_arr[i];
    struct MyRSPair *recv = (struct MyRSPair *)(args[4]);
    int recv_cnt = *((int *)(args[5]));


    //printf("decode %d %d %d recv = %d\n", size, local_size_total, global_size, recv_cnt);
    // for (int i = 0; i < local_size; i ++)
         //printf(" %d ", local_arr[i]);
    //printf("\n");

    // for (int i = 0; i < recv_cnt; i ++) {
    //     printf("recv %d\n", recv[i].id);
    // }
    int8_t *erased = MALLOC(int8_t, global_size + local_size_total + local_size);

    // memset(erased, 1, sizeof(int8_t) * global_size);
    for (int i = 0; i < global_size + local_size_total + local_size; i ++)
        erased[i] = 1;

    lrc_t     *lrc  = MALLOC(lrc_t, 1); //&(lrc_t) {0};
    lrc_buf_t *buf  = MALLOC(lrc_buf_t, 1);
    lrc->inited_ = 0;
    buf->inited_ = 0;
    if (lrc_init_n(lrc, local_size, local_arr, global_size + local_size) != 0) {
        //printf("err1\n");
        free(lrc);
        // lrc_destroy(lrc);
        // lrc_buf_destroy(buf);
        return NULL;
    }

    int t = lrc_buf_init(buf, lrc, size); 
    if (t != 0) { 
        lrc_destroy(lrc);
        free(lrc);
        free(buf);
        free(erased);
        // printf("err2 %d\n", t);
        return NULL;
    }
/////////////////////////////////////////////////////////////////////////////////////////
//modified by shawnshanks_fei, date:20170915.pm
    lrc->n = local_size_total + global_size;
/////////////////////////////////////////////////////////////////////////////////////////    

    for (int i = 0; i < recv_cnt; i ++) {
        erased[recv[i].id] = 0;
        char *dest;
        // printf("recv id = %d\n", recv[i].id);
        if (recv[i].id < local_size_total)
            dest = buf->data[recv[i].id];
        else
            dest = buf->code[recv[i].id - local_size_total];
        memcpy(dest, recv[i].symbol, size);
    }


    // for (int i = 0; i < global_size + local_size_total; i ++)
    //     printf("%d", erased[i]);
    // printf("\n");
    // printf("decoding data\n");
    // for (int i = 0; i < local_size_total; i ++) {
    //     for (int j = 0; j < size; j ++)
    //         printf("%03d ", *(buf->data[i] + j));
    //     printf("\n");
    // }
    // printf("code data\n");
    // for (int i = 0; i < global_size; i ++) {
    //     for (int j = 0; j < size; j ++)
    //         printf("%03d ", *(buf->code[i] + j));
    //     printf("\n");
    // }
    int decode_res = lrc_decode(lrc, buf, erased);
    // printf("%d\n", decode_res);
    if (decode_res != 0) {
        lrc_destroy(lrc);
        lrc_buf_destroy(buf);
        free(erased);
        free(lrc);
    //    printf("err3\n");
        free(buf);
        return NULL;
    }
    free(erased);

    lrc_destroy(lrc);
    free(lrc);
    return (void *) buf;
}
