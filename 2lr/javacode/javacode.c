#include "javacode_javacode.h"
#include <emmintrin.h>
#include <stdio.h>


JNIEXPORT void JNICALL Java_javacode_javacode_Work
  (JNIEnv *e, jobject o, jintArray img)
{
	jint *Image_p = (*e)->GetPrimitiveArrayCritical(e, img,0);
	jint box;
	jsize size = (*e)->GetArrayLength(e,img);
	int pix = 0;
	__m128i im, mask;
	mask = _mm_set_epi32(0, 0xffffffff,0, 0xffffffff);
	for (pix;pix<size;pix+=4){
		im = _mm_set_epi32(Image_p[pix],Image_p[pix+1],Image_p[pix+2],Image_p[pix+3]);
		im = _mm_xor_si128(im,mask);
		_mm_storeu_si128(Image_p+pix,im);
		box = Image_p[pix];
		Image_p[pix]=Image_p[pix+3];
		Image_p[pix+3]=box;
		box = Image_p[pix+1];
		Image_p[pix+1]=Image_p[pix+2];
		Image_p[pix+2]=box;
	}
	(*e)->ReleasePrimitiveArrayCritical(e,img,Image_p,0);
}
