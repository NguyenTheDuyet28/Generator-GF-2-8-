#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#define AES_IRREDUCIBLE_POLY 0x11B
#define FIELD_SIZE 256 // Số phần tử trong GF(2^8)
#define MAX_POWER 255  // 2^8 - 1

#define GENERATOR 0x03

uint8_t Antilog[FIELD_SIZE];
uint8_t Log[FIELD_SIZE];

uint8_t gf_multiply(uint8_t a, uint8_t b) {
    uint8_t result = 0;
    uint8_t high_bit_set;
    for (int i = 0; i < 8; i++) {
        if (b & 1) {
            result ^= a;
        }
        high_bit_set = (a & 0x80);
        a <<= 1;
        if (high_bit_set) {
            a ^= 0x1B;
        }
        b >>= 1;
    }
    return result;
}

void initialize_gf256_tables() {
    uint8_t a = 1; 
    Antilog[MAX_POWER] = 1; // g^255 = g^0 = 1
    Log[1] = 0;             

    Antilog[0] = 1;

    for (int i = 1; i < MAX_POWER; i++) {
        a = gf_multiply(a, GENERATOR);
        Antilog[i] = a;
        Log[a] = i;
    }

    Log[0] = 0;
}

uint8_t gf_inverse_generator(uint8_t a) {
    if (a == 0) {
        return 0;
    }
    uint8_t i = Log[a];
    uint8_t j = MAX_POWER - i;
    return Antilog[j];
}

int main() {
    printf("Khoi tao bang Log/Antilog cho GF(2^8) voi P(x)=0x11B...\n");
    initialize_gf256_tables();
    printf("Khoi tao hoan tat.\n");

    printf("\nBang nghich dao nhan trong GF(2^8) (Phuong phap Generator)\n");
    printf("=========================================================\n");
    printf("|  So (hex) | Nghich dao (hex) | Kiem tra (So * Nghich dao) |\n");
    printf("|-----------|--------------------|----------------------------|\n");

    // Lặp qua tất cả các phần tử khác 0
    for (int i = 1; i <= 255; i++) {
        uint8_t a = (uint8_t)i;
        uint8_t inverse = gf_inverse_generator(a);
        uint8_t check = gf_multiply(a, inverse);

        printf("|   0x%02X    |        0x%02X      |            0x%02X          |\n", a, inverse, check);

        if (check != 1) {
            printf("LOI! Phep nhan kiem tra khong bang 1 tai 0x%02X.\n", a);
            return 1;
        }
    }
    printf("=========================================================\n");
    uint8_t test_val;
    printf("Nhap vao da thuc dang HEXA (Vi du 0xFB): ");
    scanf_s("%hhx", &test_val);
    uint8_t test_inv = gf_inverse_generator(test_val);
    printf("Nghich dao cua 0x%02X la 0x%02X\n", test_val, test_inv);
    printf("Kiem tra: 0x%02X * 0x%02X = 0x%02X\n", test_val, test_inv, gf_multiply(test_val, test_inv));
    return 0;
}