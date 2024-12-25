#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "luhash.h"

// ����һ��ѧ���ṹ��
typedef struct {
	int id;         // ѧ��ID
	char name[50];  // ѧ������
	int age;        // ѧ������
	float grade;    // ѧ���ɼ�
} student_t;

// ��ϣ����غ�������

// ����һ�������ѧ������
student_t generate_student(int id) {
	student_t s;
	s.id = id;
	snprintf(s.name, sizeof(s.name), "Student%d", id);  // Ϊÿ��ѧ������Ψһ����
	s.age = 18 + (id % 10);  // ����������18��27֮��
	s.grade = 60 + (id % 40);  // ����ɼ���60��100֮��
	return s;
}

// ���Ժ���
void test_hash_table_with_students() {
	int table_size = 10;
	lu_hash_table_t* table = lu_hash_table_init(table_size);
	assert(table != NULL);  // ȷ�����ʼ���ɹ�

	// ����100��ѧ������
	for (int i = 1; i <= 100; i++) {
		student_t student = generate_student(i);
		lu_hash_table_insert(table, student.id, &student);
	}

	// ���Ҳ���֤ѧ������
	for (int i = 1; i <= 100; i++) {
		student_t* found_student = (student_t*)lu_hash_table_find(table, i);
		assert(found_student != NULL);  // ȷ��ѧ������
		assert(found_student->id == i); // ��֤ID
		printf("Found student: %d, Name: %s, Age: %d, Grade: %.2f\n",
			found_student->id, found_student->name, found_student->age, found_student->grade);
	}

	// ɾ������ѧ�����ݲ���֤
	for (int i = 1; i <= 50; i++) {
		lu_hash_table_delete(table, i);
		student_t* found_student = (student_t*)lu_hash_table_find(table, i);
		assert(found_student == NULL);  // ȷ����ɾ��
	}

	// ���δɾ����ѧ�������Ƿ���Ȼ����
	for (int i = 51; i <= 100; i++) {
		student_t* found_student = (student_t*)lu_hash_table_find(table, i);
		assert(found_student != NULL);  // ȷ��δɾ����ѧ����Ȼ����
	}

	// ���ٹ�ϣ��
	lu_hash_table_destroy(table);

	printf("Student hash table test passed!\n");
}

int main() {
	test_hash_table_with_students(); // ִ�в���
	return 0;
}