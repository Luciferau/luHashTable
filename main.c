#include "luhash.h"

int main()
{
	int keys[] = { 10, 20, 30, 40, 50 }; // ���Լ�ֵ
	int table_sizes[] = { 8, 16, 10, 32 }; // ���Թ�ϣ���С������Ϊ2���ݣ�
	int num_keys = sizeof(keys) / sizeof(keys[0]);
	int num_sizes = sizeof(table_sizes) / sizeof(table_sizes[0]);

	// �����ͷ
	printf("Key\tTable Size\tHash Value\n");
	printf("--------------------------------\n");

	// ����ÿ����ֵ�ڲ�ͬ���С�µĹ�ϣֵ
	for (int i = 0; i < num_keys; i++) {
		for (int j = 0; j < num_sizes; j++) {
			int hash_value = lu_hash_function(keys[i], table_sizes[j]);
			printf("%d\t%d\t\t%d\n", keys[i], table_sizes[j], hash_value);
		}
	}

	return 0;
}