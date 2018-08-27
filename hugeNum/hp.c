#include <stdio.h>
#include <malloc.h>

typedef unsigned char SIGN;
#define POSITIVE 			0
#define NEGATIVE			1

typedef struct HUGE{
	int *huge;		//存储巨大数的有效数字
	SIGN sign;		//存储巨大数的符号位
	int length;		//存储巨大数的长度
	int order;		//由于巨大数的阶数（主要为了处理小数）
}HUGE;

const char* charArray[4] = {"%4d", "%1d", "%2d", "%3d"};

void readNum(HUGE *hg, char *fileName);
void show(HUGE *hg);
void calculate(HUGE *hg1, HUGE *hg2, HUGE *result);
void add(HUGE hg1, HUGE hg2, HUGE *addHuge);
void sub(HUGE hg1, HUGE hg2, HUGE *subHuge);
void mul(HUGE hg1, HUGE hg2, HUGE *subHuge);

void mul(HUGE hg1, HUGE hg2, HUGE *mulHuge) {
    int minOrder;
    int maxOrder;
    int i;
    int j;
    int length;
    int *p;
    int index;
    int carry = 0;
    int temp;

    mulHuge->sign = (hg1.sign ^ hg2.sign);
    minOrder = hg1.order + hg2.order;
    mulHuge->order = minOrder;
    maxOrder = minOrder + hg1.length + hg2.length - 2;
    length = maxOrder - minOrder + 2;
    mulHuge->length = length;
    p = (int *)malloc(sizeof(int) * length);
    for(i = 0; i < length; i++) {
    	p[i] = 0;
    }
    mulHuge->huge = p;
    for(i = 0; i < hg1.length; i++) {
    	for(j = 0; j < hg2.length; j++) {
    		temp = hg1.huge[i] * hg2.huge[j];
    		index = j + i;
    		p[index] += temp;
    	}
    	for(j = 0; j < length; j++) {
    		p[j] += carry;
    		carry = p[j] / 10000;
    		p[j] = p[j] % 10000;
    	}
    }
//    for(i = 0; i < length; i++) {
//    	printf("[%04d]", p[i]);
//    }
//    printf("\n");
}

void sub(HUGE hg1, HUGE hg2, HUGE *subHuge) {
    if(hg2.sign == NEGATIVE) {
        hg2.sign = POSITIVE;
        add(hg1, hg2, subHuge);
        hg2.sign = NEGATIVE;
    }else{
        hg2.sign = NEGATIVE;
        add(hg1, hg2, subHuge);
        hg2.sign = POSITIVE;
    }
}

void show(HUGE *hg) {
    int i;

    printf("%c", hg->sign ==  POSITIVE ? 43 : 45);
    printf("%d", hg->huge[hg->length - 1]);
    if(0 - hg->order == hg->length - 1) {
            printf(".");
        }
    for(i = hg->length - 2; i > 0; i--) {
        printf("%04d", hg->huge[i]);
         if(0 - hg->order == i) {
            printf(".");
        }
    }
    printf("%d", hg->huge[0]);
    printf("\n");
}

void calculate(HUGE *hg1, HUGE *hg2, HUGE *result) {
    int i;
    int length1 = hg1->length;
    int length2 = hg2->length;
    int resultLength = length1 > length2 ? (length1 + 1) : length2 + 1;
    int *p;
    int *onesComplement1;
    int *onesComplement2;
    int temp1;
    int temp2;
    int temp;
    int carryNum;
    int carry = 0;

    p = (int *)malloc(sizeof(int) * resultLength);
    onesComplement1 = (int *)malloc(sizeof(int) * length1);
    onesComplement2 = (int *)malloc(sizeof(int) * length2);
    result->huge = p;
    result->length = hg1->length;
    result->order = hg1->order <= hg2->order ? hg1->order : hg2->order;
    if(hg1->sign == NEGATIVE) {
        for(i = 0; i < length1; i++) {
            onesComplement1[i] = 9999 -  hg1->huge[i];
        }
    }else{
        for(i = 0; i < length1; i++) {
            onesComplement1[i] = hg1->huge[i];
        }
    }
    if(hg2->sign == NEGATIVE) {
        for(i = 0; i < length2; i++) {
            onesComplement2[i] = 9999 -  hg2->huge[i];
        }
    }else{
        for(i = 0; i < length2; i++) {
            onesComplement2[i] = hg2->huge[i];
        }
    }
    for(i = 0; i < resultLength; i++) {
        temp1 = i < length1 ? onesComplement1[i] : (hg1->sign == POSITIVE ? 0 :9999);
        temp2 = i < length2 ? onesComplement2[i] : (hg2->sign == POSITIVE ? 0 :9999);
        //printf("#%d %d", temp1, temp2);
        temp = temp1 + temp2 + carry;
        carry = temp >= 10000 ? 1 : 0;
        p[i] = temp % 10000;
    }
    carryNum = carry;
    for (i = 0; i < resultLength && carry; i++) {
        p[i] +=carry;
        if (i<resultLength-1) {
            carry = p[i] / 10000;
            p[i] = p[i] % 10000;
        }
    }
    if((hg1->sign ^ hg2->sign) ^ carryNum) {
        result->sign = NEGATIVE;
        for(i = 0; i < resultLength; i++) {
            p[i] = 9999 - p[i];
        }
    }
    else{
        result->sign = POSITIVE;
    }
//    printf("%c", result->sign ==  POSITIVE ? 43 : 45);
//    for(i = 0; i < resultLength - 1; i++) {
//        printf("[%04d]", p[i]);
//    }
//    if(p[i]) {
//        printf("%d", p[i]);
//    }
//    printf("\n");

    free(onesComplement1);
    free(onesComplement2);
}

void add(HUGE hg1, HUGE hg2, HUGE *addHuge) {
    int *p1;
    int *p2;
    int length;
    int integerLength;
    int decLength;
    int i;

    decLength = hg1.order <= hg2.order ? hg1.order : hg2.order;
    decLength = 0 - decLength;
    integerLength = hg1.length + hg1.order >= hg2.length + hg2.order
                ? hg1.length + hg1.order : hg2.length + hg2.order;
    length = decLength + integerLength;
    p1 = (int *)malloc(sizeof(int) * length);
    p2 = (int *)malloc(sizeof(int) * length);
    for(i = 0; i < length; i++) {
        p1[i] = 0;
        p2[i] = 0;
    }
    //printf("*%d", p1[0]);
    //printf("*%d", hg1.length);
    if(hg1.order <= hg2.order) {
        for(i = 0; i < hg1.length; i++) {
            p1[i] = hg1.huge[i];
        }
    }else{
        for(i = 0; i < hg1.length; i++) {
            p1[i - hg2.order + hg1.order] = hg1.huge[i];
        }
    }
    if(hg1.order <= hg2.order) {
        for(i = 0; i < hg2.length; i++) {
            p2[i - hg2.order + hg1.order] = hg2.huge[i];
        }
    }else{
        for(i = 0; i < hg2.length; i++) {
            p2[i] = hg2.huge[i];
        }
    }
    hg1.length = length;
    hg2.length = length;
    hg1.huge = p1;
    hg2.huge = p2;
    calculate(&hg1, &hg2, addHuge);
    free(p1);
    free(p2);
}

void readNum(HUGE *hg, char *fileName) {
    FILE *fp;
    int i;
    int integerLength;
    int decLength;
    int length;
    char ch;
    int *p;
    int tempInt;
    int tempDel;
    int j;
    int hasSign;

    fp = fopen(fileName, "r");
    if(fp == NULL) {
        printf("error");
        return;
    }
    fseek(fp, 0L, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    ch = fgetc(fp);
	if(ch == '+') {
		hg->sign = POSITIVE;
		fseek(fp, 1L, SEEK_SET);
		hasSign = 1;
		length--;
	}else if(ch == '-') {
		hg->sign = NEGATIVE;
		fseek(fp, 1L, SEEK_SET);
		hasSign = 1;
		length--;
	}else{
		hg->sign = POSITIVE;
		hasSign = 0;
		fseek(fp, 0L, SEEK_SET);
	}
    for(i = 0; i < length; i++) {
        ch = fgetc(fp);
        if(ch == '.') {
            integerLength = i;
            length--;
        }
    }

    decLength = length - integerLength;
    tempDel = decLength % 4;
    decLength = (decLength + 3) / 4;
    hg->order = 0 - decLength;
    tempInt = integerLength % 4;
    integerLength = (integerLength + 3) / 4;
    length = decLength + integerLength;
    p = (int *)malloc(sizeof(int) * length);
    hg->huge = p;
    fseek(fp, 0L, SEEK_SET);
    if(hasSign) {
        fgetc(fp);
    }
	fscanf(fp, charArray[tempInt], &p[length - 1]);
	j = length - 1;
	for(i = 0; i < integerLength - 1; i++) {
        fscanf(fp, "%4d", &p[--j]);
	}
	fgetc(fp);
	if(tempDel) {
        decLength--;
	}
	for(i = 0; i < decLength; i++) {
        fscanf(fp, "%4d", &p[--j]);
	}
	if(tempDel) {
        fscanf(fp, charArray[tempDel], &p[--j]);
	}
	while(tempDel) {
        p[0] *= 10;
        tempDel = (tempDel + 1) % 4;
	}
	hg->length = length;
//	printf("%c", hg->sign ==  POSITIVE ? 43 : 45);
//	for(i = 0; i < hg->length; i++) {
//        printf("[%04d]", p[i]);
//	}
//	printf("%d", hg->order);
//	printf("\n");

	fclose(fp);
}

int main() {
    char *fileName1 = "./file/abc.txt";
    char *fileName2 = "./file/acc.txt";
    HUGE huge1;
    HUGE huge2;
    HUGE hugeAdd;
    HUGE hugeSub;
    HUGE hugeMul;

    readNum(&huge1, fileName1);
    printf("巨大数1为：");
    show(&huge1);
    readNum(&huge2, fileName2);
    printf("巨大数2为：");
    show(&huge2);
    add(huge1, huge2, &hugeAdd);
    printf("加法结果为：");
    show(&hugeAdd);
    sub(huge1, huge2, &hugeSub);
    printf("减法结果为：");
    show(&hugeSub);
   	mul(huge1, huge2, &hugeMul);
   	printf("乘法结果为：");
   	show(&hugeMul);

    free(huge1.huge);
	free(huge2.huge);
	free(hugeAdd.huge);
	free(hugeSub.huge);
	free(hugeMul.huge);

    return 0;
}
