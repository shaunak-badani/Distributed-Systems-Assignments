#include<bits/stdc++.h>

using namespace std;

// utility function 
void print_arr(int arr[], int l, int r) {
    for(int i = l ; i <= r ; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Returns the position of the pivot
int partition(int arr[], int l, int r) {
    // choosing pivot to be the last element
    int pivot = arr[r];
    int i = l - 1;
    for(int j = l ; j < r ; j++) {
        if(arr[j] <= pivot) {
            i += 1;
            swap(&arr[i], &arr[j]);
        }
    }
    i += 1;
    swap(&arr[i], &arr[r]);
    return i;
}

void quick_sort(int arr[], int l, int r) {
    if( l > r ) { return; }

    int q = partition(arr, l, r);
    quick_sort(arr, l, q - 1);
    quick_sort(arr, q + 1, r);
}

int main() {
    int n, a[100000];
    cin >> n;

    // Enter input of numbers
    for(int i = 0 ; i < n ; i++) { cin >> a[i]; }

    quick_sort(a, 0, 7);
    print_arr(a, 0, 7);
    return 0;
}