#include <stdio.h>
#include <stdint.h>
#include <xmmintrin.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb-master\stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb-master\stb_image_write.h"

void subvec(uint8_t*v){
	__m128 a, b;
	__declspec(align(16)) float v2[4] = {(float)255, (float)255, (float)255, (float)3[v]};
	__declspec(align(16)) float v1[4] = {(float)0[v], (float)1[v], (float)2[v], (float)0};
	
	a = _mm_load_ps(v1);
	b = _mm_load_ps(v2);
	a = _mm_sub_ps(b,a);
	_mm_store_ps(v1,a);
	0[v] = 0[v1];
	1[v] = 1[v1];
	2[v] = 2[v1];
	3[v] = 3[v1];
}

void continue_block(){
	char answer[3];
	printf("Continue? (y/n): ");
	scanf("%s", &answer);
	if (answer[0] == 121) {
		system("cls");
		main();
	}
}

int main(){
	printf("\n\n##############################\n# Welcome to Image Inverter! #\n##############################\n");
	char filename[50];
	char filetype[3];
	char answer[3];
	printf("Enter the picture name: ");
	scanf("%s", &filename);
	int width, height, bpp;
	uint8_t* rgb_image = stbi_load(filename, &width, &height, &bpp, 3);
	for (int i = 0; i < width*height*3; i+=6)
		subvec(rgb_image+i);
	printf("Enter type of output picture (png/jpg/bmp): ");
	scanf("%s", &filetype);
	if (filetype[0] == 112) stbi_write_png("ready.png", width, height, 3, rgb_image, 3*width);
	else if(filetype[0] == 106) stbi_write_jpg("ready.jpg", width, height, 3, rgb_image, width*3);
	else if(filetype[0] == 98) stbi_write_bmp("ready.bmp", width, height, 3, rgb_image);
	stbi_image_free(rgb_image);
	printf("\nCompleted!\nFile ready.%s cretaed!\n", &filetype);
	continue_block();
}
