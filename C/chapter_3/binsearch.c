#include <stdio.h>

//二分查找
int binsearch(int x,int v[],int size){
	int low,high,mid;
	low = 0;
	high = size -1;
	while(low <= high){
		mid = (low + high)/2;
		if(x < v[mid]){
			high = mid -1;
		}else if(x > v[mid]){
			low = mid + 1;
		}else{
			return mid;
		}
	}
	
	return -1;
}
