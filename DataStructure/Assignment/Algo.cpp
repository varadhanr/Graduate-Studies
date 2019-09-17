#include <stdio.h>
#include<stdlib.h>
#include<iostream>

using namespace std;

// Representation of Linked List Node
struct Node {
	int value;
	struct Node* next;
};

void swapEntireNode(struct Node ** head, struct Node * ptr1, struct Node * min,
		struct Node * prevPtr1, struct Node * prevMin) {

	if (prevPtr1 == NULL) {
		//--> we are swapping with head
		*head = min;

	} else {

		prevPtr1->next = min;

	}

	//get next of min and ptr1 for exchanging
	struct Node * nextOfMin = min->next;
	struct Node * nextOfPtr1 = (ptr1)->next;

	//If ptr1 and min are not adjacent nodes
	if (ptr1->next != min) {

		prevMin->next = ptr1;
		min->next = nextOfPtr1;

	} else {

		//if ptr1 and min are adjacent nodes
		min->next = ptr1;

	}

	(ptr1)->next = nextOfMin;

}

struct Node* selectionSort(struct Node* head) {
	struct Node * ptr1;
	struct Node * prevPtr1 = NULL;
	struct Node * ptr2;
	struct Node * min;
	struct Node * prevMin;

	for (ptr1 = head; ptr1 != NULL; ptr1 = ptr1->next) {

		min = ptr1;

		for (ptr2 = ptr1; ptr2->next != NULL; ptr2 = ptr2->next) {

			if (ptr2->next->value < min->value) {

				min = ptr2->next;
				prevMin = ptr2;

			}

		}
		//only swap if min and ptr1 are not equal
		if (min != ptr1) {

			swapEntireNode(&head, ptr1, min, prevPtr1, prevMin);

		}

		ptr1 = min;
		prevPtr1 = ptr1;

	}
	return head;
}

void push(struct Node** head_ref, int new_data) {
	struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));

	new_node->value = new_data;

	new_node->next = (*head_ref);

	(*head_ref) = new_node;
}

void print(struct Node* head) {
	while (head != NULL) {
		cout << head->value << " :" << head << "\n";
		head = head->next;
	}
}

int main() {
	struct Node* head = NULL;

	//128->5->11->18->7->48->26->32->81->72->59->27->13->9->100
	push(&head, 100);
	push(&head, 9);
	push(&head, 13);
	push(&head, 27);
	push(&head, 59);
	push(&head, 72);
	push(&head, 81);
	push(&head, 32);
	push(&head, 26);
	push(&head, 48);
	push(&head, 7);
	push(&head, 18);
	push(&head, 11);
	push(&head, 5);
	push(&head, 128);

	cout << "Linked list before sorting with address:\n";
	print(head);

	head = selectionSort(head);

	cout << "\nLinked list after sorting with address:\n";
	print(head);

	return 0;
}
