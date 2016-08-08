#include <stdio.h>
#include <stdlib.h>

void InsertSort(int arr[], int size)
{
    int tmp;
    for(int i=0;i<size-1;i++)
    {

        for(int j=i+1;j>0;j--)
        {
            if(arr[j] < arr[j-1])
            {
                tmp = arr[j];
                arr[j] = arr[j-1];
                arr[j-1] = tmp;
            }
        }
    }
}

int main()
{
    int arr[] = {8,2,4,9,3,6};
    InsertSort(arr,6);
    for(int i=0;i<6;i++){
        printf("%d ",arr[i]);
    }
}
