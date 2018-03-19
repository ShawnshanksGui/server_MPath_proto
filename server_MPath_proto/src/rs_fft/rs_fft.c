/*
Encoding/erasure decoding for Reed-Solomon codes over binary extension fields
Author: Sian-Jheng Lin (King Abdullah University of Science and Technology (KAUST), email: sianjheng.lin@kaust.edu.sa)

This program is the implementation of
Lin, Han and Chung, "Novel Polynomial Basis and Its Application to Reed-Solomon Erasure Codes," FOCS14.
(http://arxiv.org/abs/1404.3458)
*/
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "../../include/rs_fft.h"

#define GET  0 //Packet received successfully
#define LOST 1 //Packet lost in  transmission
#define MALLOC(type, size) ((type *)malloc(sizeof(type) * (size)))

typedef unsigned char GFSymbol;

#define len 8//2^len: the size of Galois field
GFSymbol mask = 0x1D; //GF(2^8): x^8 + x^4 + x^3 + x^2 + 1
GFSymbol Base[] = {1, 214, 152, 146, 86, 200, 88, 230};//Cantor basis
/*
typedef unsigned short GFSymbol;
#define len 16
GFSymbol mask = 0x2D;//x^16 + x^5 + x^3 + x^2 + 1
GFSymbol Base[len] = {1, 44234, 15374, 5694, 50562, 60718, 37196, 16402, 27800, 4312, 27250, 47360, 64952, 64308, 65336, 39198};//Cantor basis
*/

//for debugging
//#define ENABLE_DEBUG_RS_FFT

#define Size (1<<len)//Field size
#define mod (Size-1)

static GFSymbol log[Size];
static GFSymbol exp[Size];
//-----Used in decoding procedure-------
static GFSymbol skewVec[mod];//twisted factors used in FFT
static GFSymbol B[Size>>1];//factors used in formal derivative
static GFSymbol log_walsh[Size];//factors used in the evaluation of the error locator polynomial


GFSymbol mulE(GFSymbol a, GFSymbol b){//return a*exp[b] over GF(2^r)
	return a? exp[(log[a]+b &mod) + (log[a]+b >>len)]: 0;
}

void walsh(GFSymbol* data, int size){//fast Walsh–Hadamard transform over modulo mod
	for (int depart_no=1; depart_no<size; depart_no <<= 1){
		for (int j = 0; j < size; j += depart_no<<1){
			for (int i=j; i<depart_no+j; i++){
				unsigned tmp2 = data[i] + mod - data[i+depart_no];
				data[i] = (data[i] + data[i+depart_no]&mod) + (data[i] + data[i+depart_no]>>len);
				data[i+depart_no] = (tmp2&mod) + (tmp2>>len);
			}
		}
	}
	return;
}

//formal derivative of polynomial in the new basis
void formal_derivative(GFSymbol* cos, int size){
	for(int i=1; i<size; i++){
		int leng = ((i^i-1)+1)>>1;
		for(int j=i-leng; j<i; j++)
			cos[j] ^= cos[j+leng];
	}
	for(int i=size; i<Size; i<<=1)
		for(int j=0; j<size; j++)
			cos[j] ^= cos[j+i];
	return;
}

void IFLT(GFSymbol* data, int size, int index){//IFFT in the proposed basis
	for (int depart_no=1; depart_no<size; depart_no <<= 1){
		for (int j=depart_no; j < size; j += depart_no<<1){
			for (int i=j-depart_no; i<j; i++)
				data[i+depart_no] ^= data[i];

			GFSymbol skew = skewVec[j+index-1];
			if (skew != mod)
				for (int i=j-depart_no; i<j; i++)
					data[i] ^= mulE(data[i+depart_no], skew);
		}
	}
	return;
}

void FLT(GFSymbol* data, int size, int index){//FFT in the proposed basis
	for(int depart_no = size>>1; depart_no > 0; depart_no >>= 1){
		for (int j = depart_no; j < size; j += depart_no<<1){
			GFSymbol skew = skewVec[j+index-1];
			if (skew != mod)
				for (int i=j-depart_no; i<j; i++)
					data[i] ^= mulE(data[i+depart_no], skew);
			for (int i=j-depart_no; i<j; i++)
				data[i+depart_no] ^= data[i];
		}
	}
	return;
}

void init(){//initialize log[], exp[]
	GFSymbol mas = (1<<len-1)-1;
	GFSymbol state=1;
	for(int i=0; i<mod; i++){
		exp[state]=i;
        if(state>>len-1){
        	state &= mas;
        	state = state<<1^mask;
        }else
        	state <<= 1;
    }
    exp[0] = mod;

    log[0] = 0;
	for(int i=0; i<len; i++)
		for(int j=0; j<1<<i; j++)
			log[j+(1<<i)] = log[j] ^ Base[i];
    for(int i=0; i<Size; i++)
        log[i]=exp[log[i]];

    for(int i=0; i<Size; i++)
        exp[log[i]]=i;
    exp[mod] = exp[0];
}


void init_dec(){//initialize skewVec[], B[], log_walsh[]
	GFSymbol base[len-1];

	for(int i=1; i<len; i++)
		base[i-1] = 1<<i;

	for(int m=0; m<len-1; m++){
		int step = 1<<(m+1);
		skewVec[(1<<m)-1] = 0;
		for(int i=m; i<len-1; i++){
			int s = 1<<(i+1);
			for(int j=(1<<m)-1; j<s; j+=step)
				skewVec[j+s] = skewVec[j] ^ base[i];
		}
		base[m] = mod-log[mulE(base[m], log[base[m]^1])];
		for(int i=m+1; i<len-1; i++)
			base[i] = mulE(base[i], (log[base[i]^1]+base[m])%mod);
	}
	for(int i=0; i<Size; i++)
		skewVec[i] = log[skewVec[i]];

	base[0] = mod-base[0];
	for(int i=1; i<len-1; i++)
		base[i] = (mod-base[i]+base[i-1])%mod;

	B[0] = 0;
	for(int i=0; i<len-1; i++){
		int depart = 1<<i;
		for(int j=0; j<depart; j++)
			B[j+depart] = (B[j] + base[i])%mod;
	}

	memcpy(log_walsh, log, Size*sizeof(GFSymbol));
	log_walsh[0] = 0;
	walsh(log_walsh, Size);
}

//Encoding alg for k/n<0.5: message is a power of two
void encodeL(GFSymbol* data, int k, GFSymbol* codeword){
	memcpy(codeword, data, sizeof(GFSymbol)*k);
	IFLT(codeword, k, 0);
	for(int i=k; i<Size; i+=k){
		memcpy(&codeword[i], codeword, sizeof(GFSymbol)*k);
		FLT(&codeword[i], k, i);
	}
	memcpy(codeword, data, sizeof(GFSymbol)*k);
	return;
}

//Encoding alg for k/n>0.5: parity is a power of two.
void encodeH(GFSymbol* data, int k, GFSymbol* parity, GFSymbol* mem){
//data: message array. parity: parity array. mem: buffer(size>= n-k)
	int t = Size-k;
	memset(parity, 0, sizeof(GFSymbol)*t);
	for(int i=t; i<Size; i+=t){
		memcpy(mem, &data[i-t], sizeof(GFSymbol)*t);
		IFLT(mem, t, i);
		for(int j=0; j<t; j++)
			parity[j] ^= mem[j];
	}
	FLT(parity, t, 0);
	return;
}

//Compute the evaluations of the error locator polynomial
void decode_init(Indicator_Type* erasure, GFSymbol* log_walsh2){
	for(int i=0; i<Size; i++)
		log_walsh2[i] = erasure[i];
	walsh(log_walsh2, Size);
	for (int i=0; i<Size; i++)
		log_walsh2[i] = (unsigned long)log_walsh2[i]*log_walsh[i]%mod;
	walsh(log_walsh2,Size);
	for (int i=0; i<Size; i++)
		if(erasure[i]) log_walsh2[i] = mod-log_walsh2[i];
}

void decode_main(GFSymbol* codeword, Indicator_Type* erasure,
				 GFSymbol* log_walsh2){
	int k2 = Size;//k2 can be replaced with k
	for (int i=0; i<Size; i++)
		codeword[i] = erasure[i]? 0 : mulE(codeword[i], log_walsh2[i]);
	IFLT(codeword, Size, 0);

	for(int i=0; i<Size; i+=2){//formal derivative
		codeword[i] = mulE(codeword[i], mod-B[i>>1]);
		codeword[i+1] = mulE(codeword[i+1], mod-B[i>>1]);
	}
	formal_derivative(codeword, k2);
	for(int i=0; i<k2; i+=2){
		codeword[i] = mulE(codeword[i], B[i>>1]);
		codeword[i+1] = mulE(codeword[i+1], B[i>>1]);
	}

	FLT(codeword, k2, 0);
	for (int i=0; i<k2; i++)
		codeword[i] = erasure[i]? mulE(codeword[i], log_walsh2[i]) : 0;
}


//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei
//Date:        20180201
//Description: the core encoding proccedure of FFT_RS, which calls 
//		       the original FFT_RS lib  
//Parameter:  
//		       param_encd.S is Symbol size, param_encd.K denotes Block  
//             size  and actually both S and K are upper cases
//==========================================================================
void *encode_FFT_RS(char *data_src, struct Param_Encd param_encd) {
	GFSymbol elem_procs[Size] = {'\0'};
	GFSymbol codeword[Size]   = {'\0'};

	GFSymbol *data_dst        = MALLOC(char, Size*param_encd.S);
	

	for(int i = 0; i < param_encd.S; i++) {
		for(int j = Size-param_encd.K; j < Size; j++) {
			elem_procs[j] = data_src[(j-(Size-param_encd.K))*param_encd.S+i];
		}
		encodeH(&elem_procs[Size-param_encd.K], param_encd.K, elem_procs, codeword);
//		memcpy(codeword, elem_procs, sizeof(GFSymbol)*Size);
		for(int j = 0; j < Size; j++) {
			data_dst[j*param_encd.S+i] = elem_procs[j];
		}
	}
//	printf("encoding data is:\n %s\n\n\n", data_dst);

	return data_dst;
}
//==========================================================================


//==========================================================================
//==========================================================================
//Author:      shawnshanks_fei
//Date:        20180201
//Description: 
//		the core decoding proccedure of FFT_RS, which calls 
//		the original FFT_RS lib 
//Parameters:  
//		 param_encd.S is Symbol size, param_encd.K denotes Block size
//       and actually both S and K are upper cases,
//       decode one element every time stored in 'elem_procs' temporarily.
//==========================================================================
void *decode_FFT_RS(struct Data_Remain data_remain, 
	                struct Param_Decd param_decd) {
//Array indicating erasures
	Indicator_Type    erasure_a[Size]  = {0};
	char     elem_procs[Size] = {'\0'};
	GFSymbol log_walsh2[Size] = {'\0'};

	char     *data_dst        = MALLOC(char, param_decd.K*param_decd.S);

	for(int i = 0; i < Size; i++) {
		erasure_a[i] = data_remain.erasure[i];
	}
	decode_init(erasure_a, log_walsh2);//Evaluate error locator polynomial

	for(int i = 0; i < param_decd.S; i++) {
		for(int j = 0; j < Size; j++) {
			if(GET == erasure_a[j]) {
			//copy data from data_remain to elem_procs if GET
				elem_procs[j] = data_remain.data[j][i];
			//(1)copy the data which is'nt lost,from data_remain to data_destination
				if(j >= param_decd.K) {
					data_dst[(j-param_decd.K)*param_decd.S+i] = data_remain.data[j][i];
				}
			}
			else {elem_procs[j] = 0;} //keep default value if LOST
		}
		//----------main processing fucntion-----------
		decode_main(elem_procs, erasure_a, log_walsh2);
		//(2)copy the data recovered from elem_procs to data_destination
		for(int j = param_decd.K; j < Size; j++) {
			if(LOST == erasure_a[j]) {
				data_dst[(j-param_decd.K)*param_decd.S+i] = elem_procs[j];
			}
		}		
	}

#ifdef ENABLE_DEBUG_RS_FFT
	printf("recovery data is:\n%s\n\n", data_dst);
#endif

	return data_dst;
}
//==========================================================================


#ifdef ENABLE_DEBUG_RS_FFT
//==========================================================================
//==========================================================================
//Author: shawnshanks_fei
//date:   20180202 
//Description: test RS-FFT encoding and decoding one block which is 
//             discomposed of many symbols(or coding elements)
int main() {
	char   data_src[128*10]         = {'\0'};
	char   *data_encd               = NULL;
	char   *data_recovery           = NULL;
	struct Param_Encd param_encd      = {10, 128};
	struct Param_Decd param_decd      = {10, 128};
	struct Data_Remain data_remain;

	init();
	init_dec();	
//encoding 
	char num = 97;
	for(int i = 0; i < 128; i++) {
		memset(&data_src[i*10], num, 10);
		num++;
		if(122 == num) {num = 97;}
	} 
	printf("\n\n");

	data_encd = encode_FFT_RS(data_src, param_encd);
//	printf("%s\n\n", data_encd);

//simulate erasure
	srand(time(NULL));
	for(int i = 0; i < Size; i++) {
		if((rand()%100/100.0) < 0.3) {
			data_remain.erasure[i] = LOST;
		}
		else {data_remain.erasure[i] = GET;}
	}
	for(int i = 0; i < Size; i++) {
		if(GET == data_remain.erasure[i]) {
			memcpy(data_remain.data[i], &data_encd[i*param_decd.S], param_decd.S);
		}
	}
//decoding
	data_recovery = decode_FFT_RS(data_remain, param_decd); 

	free(data_encd);
	free(data_recovery);

	return 0;
}

#endif
//==========================================================================

/*
void test(int k){
	//-----------Generating message----------
	GFSymbol data[Size] = {0};//message array
	srand(time(NULL));
	for(int i=Size-k; i<Size; i++)
		data[i] = rand()&mod;//filled with random numbers

	printf("Message(First n-k are zeros): \n");
	for(int i=0; i<Size; i++)
		printf("%02X ", data[i]);
	printf("\n");

	//---------encoding----------
	GFSymbol codeword[Size];
	encodeH(&data[Size-k], k, &data, codeword);
	//encodeL(data, k, codeword);

	memcpy(codeword, data, sizeof(GFSymbol)*Size);

	printf("Data:\n");
	for(int i=0; i<Size; i++)
		printf("%02X ", data[i]);
	printf("\n");


	printf("Codeword:\n");
	for(int i=0; i<Size; i++)
		printf("%02X ", codeword[i]);
	printf("\n");

	//--------erasure simulation---------
	_Bool erasure[Size] = {0};//Array indicating erasures
	for(int i=k; i<Size; i++)
		erasure[i] = 1;

	for(int i=Size-1; i>0; i--){//permuting the erasure array
		int pos = rand()%(i+1);
		if(i != pos){
			_Bool tmp = erasure[i];
			erasure[i] = erasure[pos];
			erasure[pos] = tmp;
		}
	}

	for (int i=0; i<Size; i++)//erasure codeword symbols
		if(erasure[i]) codeword[i] = 0;

	printf("Erasure (XX is erasure):\n");
	for(int i=0; i<Size; i++){
		if(erasure[i]) printf("XX ");
		else printf("%02X ", codeword[i]);
	}
	printf("\n");


	//---------Erasure decoding----------------
	GFSymbol log_walsh2[Size];
	decode_init(erasure, log_walsh2);//Evaluate error locator polynomial
	//---------main processing----------
	decode_main(codeword, erasure, log_walsh2);

	printf("Decoded result:\n");
	for(int i=0; i<Size; i++){
		if(erasure[i]) printf("%02X ", codeword[i]);
		else printf("XX ");
	}
	printf("\n");

	for (int i=0; i<Size; i++){//Check the correctness of the result
		if(erasure[i] == 1)
			if(data[i] != codeword[i]){
				printf("Decoding Error!\n");
				return;
			}
	}
	printf("Decoding is successful!\n");
	return;
}

int main(){
	init();//fill log table and exp table
	init_dec();//compute factors used in erasure decoder
	test(Size/2);//test(int k), k: message size
	return 1;
}
*/
