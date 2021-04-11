/*
 * Created April, 11 2021
 * By Squanix Machine
 *
 * This code aimed to demonstrated AES encryption using ESP32. This code was able compiled with ESP IDF.
 *
 * You can compile this code using Arduino IDE with some little changes :
 * 		1. Replace the 'printf' code by 'Serial.print'
 * 		2. Replace 'void app' by 'void loop' or 'void setup',
 *
 */
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "mbedtls/aes.h"
#include <string.h>

void encrypt(char * plainText, char * key, unsigned char * outputBuffer){

  mbedtls_aes_context aes;
  mbedtls_aes_init( &aes );
  mbedtls_aes_setkey_enc( &aes, (const unsigned char*) key, strlen(key) * 8 );
  mbedtls_aes_crypt_ecb( &aes, MBEDTLS_AES_ENCRYPT, (const unsigned char*)plainText, outputBuffer);
  mbedtls_aes_free( &aes );
}

void decrypt(unsigned char * chipherText, char * key, unsigned char * outputBuffer){

  mbedtls_aes_context aes;

  mbedtls_aes_init( &aes );
  mbedtls_aes_setkey_dec( &aes, (const unsigned char*) key, strlen(key) * 8 );
  mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, (const unsigned char*)chipherText, outputBuffer);
  mbedtls_aes_free( &aes );
}

void app_main(void)
{
    printf("AES Encryptor and Decryptor\n");
    char * key = "asdfghertyuizsdr";

	char *plainText = "Thanks to Squanix Machine who was create this code for us. Now you can use this code to create your secure communication using ESP32";

	/*
	 * AES is 128 block enc, we need to separated message for every 16 bytes.
	 */
	uint16_t len_chiper = (strlen(plainText)/16 + 1) * 16;
	unsigned char cipherTextOutput[len_chiper];
	unsigned char decipheredTextOutput[len_chiper];
	unsigned char decipheredBlockOutput[len_chiper];
	char gap = '@';
	char * chp = malloc(len_chiper);
	strcpy(chp, "");

	for(int loop = 0; loop < (len_chiper / 16); loop++){
		char plaintext_tmp[16];
		int index = loop * 16;
		for (int i = 0; i < 16 ; i++){

			if((i + index) > strlen(plainText)){

				plaintext_tmp[i] = gap;
			}
			else plaintext_tmp[i] = plainText[i + index];
		}

		encrypt(plaintext_tmp, key, cipherTextOutput);

		for (int i = 0; i < 16; i++) {
			char str[3];
			sprintf(str, "%02x", (int)cipherTextOutput[i]);
			strcat(chp, str);
		}

		decrypt(cipherTextOutput, key, decipheredTextOutput);

		for (int i = 0; i < 16; i++) {
			decipheredBlockOutput[i+ index] = decipheredTextOutput[i];
		}
	}
	printf("\n Cipher Text %s \n", chp);
	printf("\n Plain Text After Decode %s \n", decipheredBlockOutput);
}
