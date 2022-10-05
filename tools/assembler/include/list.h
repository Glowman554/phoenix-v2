#pragma once

#include <string.h>
#include <stdlib.h>

template <typename T>
class list {
public:

	struct node {
		T data;
		bool taken = false;
	};

	list(int initial_length) {
		length = initial_length;
		data = (node*)malloc((length + 1) * sizeof(node));
		memset(data, 0, length * sizeof(node));
	};

	~list() {
		free(data);
	}

	template <typename Y>
	node* find(bool (*f)(Y, node*), Y arg) {
		for (size_t i = 0; i < length; i++) {
			if (data[i].taken) {
				if (f(arg, &data[i])) {
					return &data[i];
				}
			}
		}

		return nullptr;
	}

	template <typename Y>
	void foreach(void (*f)(Y, node*), Y arg) {
		for (size_t i = 0; i < length; i++) {
			if (data[i].taken) {
				f(arg, &data[i]);
			}
		}
	}

	void remove(node* n) {
		for (size_t i = 0; i < length; i++) {
			if (&data[i] == n) {
				memset(&data[i], 0, sizeof(node));
			}
		}
	}

	node* add(T d) {
	retry:
		for (size_t i = 0; i < length; i++) {
			if (!data[i].taken) {
				data[i].data = d;
				data[i].taken = true;
				return &data[i];
			}
		}

		data = (node*)realloc(data, (length + 2) * sizeof(node));
		memset(&data[length], 0, sizeof(node));
		length++;
		goto retry;
	}

	int length;
	node* data;
};