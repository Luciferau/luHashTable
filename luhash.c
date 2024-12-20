#include "luhash.h"

/**
 * @brief Computes a hash value for a given key using the multiplication method.
 *
 * This function implements a hash function based on the multiplication method,
 * using the fractional part of the product between the key and the constant
 * A (the reciprocal of the golden ratio). If the table size is a power of two,
 * the modulo operation is optimized using bitwise operations. Otherwise, a
 * standard modulo operation is applied.
 *
 * @param key The integer key to be hashed.
 * @param table_size The size of the hash table (number of buckets).
 * @return The computed hash value, ranging from 0 to table_size - 1.
 */
int lu_hash_function(int key, int table_size)
{
	static const float golden_rate_reciprocal = 0.6180339887; // Reciprocal of the golden ratio

	double temp = key * golden_rate_reciprocal;
	double fractional_part = temp - (int)temp; // Extract fractional part
	int hash = (int)(table_size * fractional_part);

	// Optimize modulo operation if table_size is a power of two
	if ((table_size & (table_size - 1)) == 0) {
		return hash & (table_size - 1); // Use bitwise AND for power-of-two table sizes
	}

	// Fallback to standard modulo operation
	return hash % table_size;
}

/**
 * Initializes a hash table with the specified number of buckets.
 * If the specified `table_size` is invalid (less than or equal to 0),
 * a default size is used instead (`LU_HASH_TABLE_DEFAULT_SIZE`).
 *
 * Memory is allocated for the hash table structure and the buckets.
 * Each bucket is initialized as a linked list by default.
 *
 * @param table_size The number of buckets in the hash table. If <= 0, default size is used.
 * @return A pointer to the newly initialized hash table, or exits the program if memory allocation fails.
 *
 * Usage example:
 *     lu_hash_table_t* hash_table = lu_hash_table_init(16); // Creates a hash table with 16 buckets.
 */
lu_hash_table_t* lu_hash_table_init(int table_size)
{
	if (table_size <= 0) {
		table_size = LU_HASH_TABLE_DEFAULT_SIZE;
	}
	lu_hash_table_t* table = (lu_hash_table_t*)LU_MM_MALLOC(sizeof(lu_hash_table_t));
	table->element_count = 0;
	table->buckets = (lu_hash_bucket_t*)LU_MM_CALLOC(table_size, sizeof(lu_hash_bucket_t));
	table->table_size = table_size;

	for (size_t i = 0; i < table_size; i++) {
		table->buckets[i].bucket_type = LU_HASH_BUCKET_LIST;
		table->buckets[i].data.list_head = NULL;
	}

	return table;
}

/**
*
*/
void lu_hash_table_insert(lu_hash_table_t* table, int key, void* value)
{
	int index = lu_hash_function(key, table->table_size);
	lu_hash_bucket_t* bucket = &table->buckets[index];
	if (LU_HASH_BUCKET_LIST == bucket->bucket_type) {
		lu_hash_bucket_node_ptr_t new_node = (lu_hash_bucket_node_ptr_t)LU_MM_MALLOC(sizeof(lu_hash_bucket_node_t));
		if (!new_node) {
#ifdef LU_HASH_DEBUG
			printf("Memory allocation  failed for new code\n");
#endif // LU_HASH_DEBUG
			return;
		}

		// Check if the key already exists and update the value
		lu_hash_bucket_node_t* current = bucket->data.list_head;
		while (current) {
			if (current->key == key) {
				current->value = value; // Update value if key exists
				return;
			}
			current = current->next;
		}

		new_node->value = value;
		new_node->key = key;
		new_node->next = bucket->data.list_head;
		bucket->data.list_head = new_node;//update the head point
		table->element_count++;
	}
}