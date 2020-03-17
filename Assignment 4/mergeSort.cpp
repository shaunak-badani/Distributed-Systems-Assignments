#include <bits/stdc++.h>

using namespace std;

void merge(int arr[], int l, int mid, int r) {
    int i, j;
    i = l;
    j = mid + 1;
    int k = 0;
    int temp_arr[r - l + 1];
    while( i <= mid && j <= r) {
        if(arr[i] < arr[j]) {
            temp_arr[k++] = arr[i++];
        }
        else {
            temp_arr[k++] = arr[j++];
        }
    }
    while(i <= mid) {
        temp_arr[k++] = arr[i++];
    }
    while(j <= r) {
        temp_arr[k++] = arr[j++];
    }

    for(int sal = 0 ; sal < k; sal++ ) {
        arr[l + sal] = temp_arr[sal];
    }
}

void mergeSort(int arr[], int l, int r) {
    if(l < r) {
        int mid = (l + r) / 2;
        mergeSort(arr, l, mid);
        mergeSort(arr, mid + 1, r);
        merge(arr, l, mid, r);
    }
}

int main() {
    int a[10000], n;
    cin >> n;
    for(int i = 0 ; i < n ; i++)
        cin >> a[i];
    mergeSort(a, 0, n-1);
    for(int i = 0 ; i < n ; i++) 
        cout << a[i] << " ";
    cout << endl;
    return 0;
}