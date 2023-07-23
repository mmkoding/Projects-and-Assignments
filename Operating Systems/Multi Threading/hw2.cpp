#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include "hw2_output.h"

using namespace std;

struct AddRowArgs {
    const vector<vector<int>> *Matrix1;
    const vector<vector<int>> *Matrix2;
    vector<vector<int>> *result;
    int row;
    unsigned matrix_id;
};

struct MultRowArgs {
    const vector<vector<int>> *MatrixAdd1;
    const vector<vector<int>> *MatrixAdd2;
    vector<vector<int>> *MatrixMult;
    int row;
    int k;
};

sem_t *row_semaphores;

void print_matrix(int M, int N, const vector<vector<int>> &Matrix) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cout << Matrix[i][j];
            if (j != N - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }
}

void *add_row(void *args) {
    AddRowArgs *a = (AddRowArgs *)args;
    int cols = a->Matrix1->at(0).size();
    for (int col = 0; col < cols; col++) {
        a->result->at(a->row)[col] = a->Matrix1->at(a->row)[col] + a->Matrix2->at(a->row)[col];
        hw2_write_output(a->matrix_id, a->row + 1, col + 1, a->result->at(a->row)[col]);
    }
    sem_post(&row_semaphores[a->row]);
    return nullptr;
}

void *mult_row(void *args) {
    MultRowArgs *a = (MultRowArgs *)args;
    sem_wait(&row_semaphores[a->row]);

    int N3 = a->MatrixAdd2->at(0).size();
    for (int col = 0; col < N3; col++) {
        int sum = 0;
        for (int i = 0; i < a->k; i++) {
            sum += a->MatrixAdd1->at(a->row)[i] * a->MatrixAdd2->at(i)[col];
        }
        a->MatrixMult->at(a->row)[col] = sum;
        hw2_write_output(2, a->row + 1, col + 1, a->MatrixMult->at(a->row)[col]);
    }

    return nullptr;
}

int main() {
    hw2_init_output();

    // Read in the dimensions and initialize the first matrix
    int M1, N1;
    cin >> M1 >> N1;
    vector<vector<int>> Matrix1(M1, vector<int>(N1));
    for (int i = 0; i < M1; i++) {
        for (int j = 0; j < N1; j++) {
            cin >> Matrix1[i][j];
        }
    }

    // Read in the dimensions and initialize the second matrix
    int M2, N2;
    cin >> M2 >> N2;
    vector<vector<int>> Matrix2(M2, vector<int>(N2));
    for (int i = 0; i < M2; i++) {
        for (int j = 0; j < N2; j++) {
            cin >> Matrix2[i][j];
        }
    }

    // Read in the dimensions and initialize the third matrix
   
	int M3, N3;
	cin >> M3 >> N3;
	vector<vector<int>> Matrix3(M3, vector<int>(N3));
	for (int i = 0; i < M3; i++) {
		for (int j = 0; j < N3; j++) {
			cin >> Matrix3[i][j];
		}
	}

	// Read in the dimensions and initialize the fourth matrix
	int M4, N4;
	cin >> M4 >> N4;
	vector<vector<int>> Matrix4(M4, vector<int>(N4));
	for (int i = 0; i < M4; i++) {
		for (int j = 0; j < N4; j++) {
			cin >> Matrix4[i][j];
		}
	}

	// Initialize the result matrices and row/column ready flags
	vector<vector<int>> MatrixAdd1(M1, vector<int>(N1));
	vector<vector<int>> MatrixAdd2(M3, vector<int>(N3));
	vector<vector<int>> MatrixMult(M1, vector<int>(N3));
	row_semaphores = new sem_t[M1 + M3];

	// Initialize semaphores
	for (int i = 0; i < M1 + M3; i++) {
		sem_init(&row_semaphores[i], 0, 0);
	}

	// Create and start the threads for addition
	vector<pthread_t> add_threads(M1 + M3);
	for (int i = 0; i < M1; i++) {
		AddRowArgs *args = new AddRowArgs{&Matrix1, &Matrix2, &MatrixAdd1, i, 0};
		pthread_create(&add_threads[i], nullptr, add_row, args);
	}
	for (int i = 0; i < M3; i++) {
		AddRowArgs *args = new AddRowArgs{&Matrix3, &Matrix4, &MatrixAdd2, i, 1};
		pthread_create(&add_threads[M1 + i], nullptr, add_row, args);
	}

	// Join the addition threads
	for (pthread_t &t : add_threads) {
		pthread_join(t, nullptr);
	}

	// Create and start the threads for multiplication
	vector<pthread_t> mult_threads(M1);
	for (int i = 0; i < M1; i++) {
		MultRowArgs *args = new MultRowArgs{&MatrixAdd1, &MatrixAdd2, &MatrixMult, i, N1};
		pthread_create(&mult_threads[i], nullptr, mult_row, args);
	}

	// Join the multiplication threads
	for (pthread_t &t : mult_threads) {
		pthread_join(t, nullptr);
	}

	print_matrix(M1, N3, MatrixMult);

	// Clean up semaphores
	for (int i = 0; i < M1 + M3; i++) {
		sem_destroy(&row_semaphores[i]);
	}
	delete[] row_semaphores;

	return 0;
}