#include <cstdio>
#include <stdlib.h>
#include <time.h>

// Holds the value of a max
template <typename Type> struct Result {
    Type value;  // the actual value of the
    int  start;  // the start index for the max subarray
    int  end;    // the end index for the max subarray
};

// MaxSubArray is a templated class that contains several algorithms for 
// calculating the max sum of a contiguous subarray of an array.
//
// Each algorithm returns a Result struct with the calculated max sum,
// as well as the start and end indices of the max subarray.  If there
// are no values in the array with a value >= 0, the Result struct will
// have a value of 0 and start > end (indicating no subarray).
//
// The template Type parameter must be an integral or floating point type
template <typename Type> class MaxSubArray {
    public:
        Result<Type> algorithmA( Type array[], int length );
        Result<Type> algorithmB( Type array[], int length );
        Result<Type> algorithmC( Type array[], int low, int high );
        Result<Type> algorithmD( Type array[], int length );
};

// algorithmA implements a simple, iterative method of calculating the max sub array
template <typename Type>
Result<Type> MaxSubArray<Type>::algorithmA ( Type array[], int length ) {
    Type maxSum = -1, sum;
    int i, j, k, start, end;

    Result<Type> result;
    for ( i = 0; i < length; i++ ) {
        for ( j = i; j < length; j++ ) {
            sum = 0;
            for ( k = i; k <= j; k++ ) {
                sum += array[ k ];
            }
            if ( sum > maxSum ) {
                maxSum = sum;
                start = i;
                end = j;
            }
        }
    }

    if ( maxSum < 0 ) {
        maxSum = 0;
        start = length;
        end = 0;
    }

    result.value = maxSum;
    result.start = start;
    result.end = end;

    return result;
}

// algorithmB implements a slightly improved iterative method of calculating
// the max sub array
template <typename Type>
Result<Type> MaxSubArray<Type>::algorithmB ( Type array[], int length ) {
    Type maxSum = -1, sum;
    int i, j, start, end;

    Result<Type> result;
    for ( i = 0; i < length; i++ ) {
        sum = 0;
        for ( j = i; j < length; j++ ) {
            sum += array[ j ];
            if ( sum > maxSum ) {
                maxSum = sum;
                start = i;
                end = j;
            }
        }
    }


    if ( maxSum < 0 ) {
        maxSum = 0;
        start = length;
        end = 0;
    }

    result.value = maxSum;
    result.start = start;
    result.end = end;

    return result;
}

// algorithmC implements a recursive, divide and conquer method of calculating
// the max sub array
template <typename Type>
Result<Type> MaxSubArray<Type>::algorithmC ( Type array[], int low, int high ) {
    Type leftMax = -1;
    Type rightMax = -1;
    Type sum = 0;
    int center = ( low + high ) / 2;
    int i;
    int leftIdx = center;
    int rightIdx = center + 1;

    Result<Type> right, middle, left;

    // base case
    if ( low == high ) {
        middle.value = array[ low ];
        middle.start = low;
        middle.end = high;
        return middle;
    }

    // recursive case
    for ( i = center; i >= low; i-- ) {
        sum += array[ i ];
        if ( sum > leftMax ) {
            leftMax = sum;
            leftIdx = i;
        }
    }

    sum = 0;
    for ( i = center + 1; i <= high; i++ ) {
        sum += array[ i ];
        if ( sum > rightMax ) {
            rightMax = sum;
            rightIdx = i;
        }
    }

    middle.start = leftIdx;
    middle.end = rightIdx;
    middle.value = leftMax + rightMax;

    if ( middle.value < 0 ) {
        middle.value = 0;
        middle.start = high + 1;
        middle.end = low;
    }

    left = this->algorithmC( array, low, center );
    right = this->algorithmC( array, center + 1, high );

    if ( left.value > right.value && left.value > middle.value ) {
        return left;
    }
    if ( right.value > middle.value ) {
        return right;
    }
    return left.value > middle.value ? left : middle;
}

// algorithmD implements a linear time, DP style method of calculating
// the max sub array
template <typename Type>
Result<Type> MaxSubArray<Type>::algorithmD ( Type array[], int length ) {
    Type maxSum = array[ 0 ];
    Type runningSum = array[ 0 ];
    int i;
    int left = 0;
    int right = 0;
    int currentLeft = 0;

    Result<Type> result;

    for ( i = 1; i < length; i++ ) {
        if ( array[ i ] > runningSum + array[ i ] ) {
            runningSum = array[ i ];
            currentLeft = i;
        } else {
            runningSum += array[ i ];
        }

        if ( runningSum > maxSum ) {
            left = currentLeft;
            right = i;
            maxSum = runningSum;
        }
    }

    if ( maxSum < 0 ) {
        maxSum = 0;
        left = length;
        right = 0;
    }
    result.value = maxSum;
    result.start = left;
    result.end = right;

    return result;
}


// calculates and prints results for various arrays using all four algorithms
void test ( ) {

    MaxSubArray<int> *intTester = new MaxSubArray<int>( );
    MaxSubArray<float> *floatTester = new MaxSubArray<float>( );

    int test1[] = {1, 4, -9, 8, 1, 3, 3, 1, -1, -4, -6, 2, 8, 19, -10, -11};
    int test2[] = {2, 9, 8, 6, 5, -11, 9, -11, 7, 5, -1, -8, -3, 7, -2};
    int test3[] = {10, -11, -1, -9, 33, -45, 23, 24, -1, -7 - 8, 19};
    int test4[] = {31, -41, 59, 26, -53, 58, 97, -93, -23, 84};
    int test5[] = {3, 2, 1, 1, -9, 1, 1, 2, 3};
    int test6[] = {12, 99, 99, -99, -27, 0, 0, 0, -3, 10};
    int test7[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    float test8[] = {-1.3, 2.77, -2, 12.8};
    int test9[] = {-1, -3, -5};


    int arrSize = 110000;
    srand( time(NULL) );
    time_t startT, endT;
    clock_t startClock, endClock;
    double elapsed_secs;

	printf("For n = %d\n", arrSize);

    for(int i = 0; i < 10; i++){
	int randTest[arrSize];
	for(int y = 0; y < arrSize; y++){
	    randTest[y] = (rand() % 19999 + (-9999));
	}
	//startT = time(NULL);
	//startClock = clock();
	//Result<int> randA = intTester->algorithmA( randTest, arrSize );
	//Result<int> T = intTester->algorithmB( randTest, 10000 );
        //Result<int> Q = intTester->algorithmC( randTest, 0, 9999 );
        //Result<int> U = intTester->algorithmD( randTest, 10000 );
	//endT = time(NULL);
	//long elapsedT = ((long)endT - (long)startT);
	//endClock = clock();
	//elapsed_secs = double(endClock - startClock)/CLOCKS_PER_SEC;
	//printf("Rand Test %d:\n", i);
    	//printf( "Algorithm A: value: %d; start: %d; end: %d\n", randA.value, randA.start, randA.end );
	//printf("Time elapsed: %f\n\n", elapsed_secs);
	//printf("Time elapsed: %f  begin: %f  end: %f\n\n", elapsed_secs, (double)startClock, (double)endClock);//elapsedT, (long)startT, (long)endT);
	
	startClock = clock();
	Result<int> randB = intTester->algorithmB( randTest, arrSize );
	endClock = clock();
	elapsed_secs = double(endClock - startClock)/CLOCKS_PER_SEC;
	printf( "Algorithm B: value: %d; start: %d; end: %d\n", randB.value, randB.start, randB.end );
        printf("Time elapsed: %f\n\n", elapsed_secs);
	//printf("Time elapsed: %f  begin: %f  end: %f\n\n", elapsed_secs, (double)startClock, (double)endClock);
	
	/*
	startClock = clock();
        Result<int> randC = intTester->algorithmC( randTest, 0, arrSize );
	endClock = clock();
        elapsed_secs = double(endClock - startClock)/CLOCKS_PER_SEC;
	printf( "Algorithm C: value: %d; start: %d; end: %d\n", randC.value, randC.start, randC.end );
        printf("Time elapsed: %f\n\n", elapsed_secs);
	//printf("Time elapsed: %f  begin: %f  end: %f\n\n", elapsed_secs, (double)startClock, (double)endClock);

	startClock = clock();
        Result<int> randD = intTester->algorithmD( randTest, arrSize );
        endClock = clock();
        elapsed_secs = double(endClock - startClock)/CLOCKS_PER_SEC;
        printf( "Algorithm D: value: %d; start: %d; end: %d\n", randD.value, randD.start, randD.end );
        printf("Time elapsed: %f\n\n\n\n", elapsed_secs);
	//printf("Time elapsed: %f  begin: %f  end: %f\n\n", elapsed_secs, (double)startClock, (double)endClock);
	*/

    }


    Result<int> A = intTester->algorithmA( test1, 16 );
    Result<int> B = intTester->algorithmB( test1, 16 );
    Result<int> C = intTester->algorithmC( test1, 0, 15 );
    Result<int> D = intTester->algorithmD( test1, 16 );

    printf("Test 1:\n");
    printf( "Algorithm A: value: %d; start: %d; end: %d\n", A.value, A.start, A.end );
    printf( "Algorithm B: value: %d; start: %d; end: %d\n", B.value, B.start, B.end );
    printf( "Algorithm C: value: %d; start: %d; end: %d\n", C.value, C.start, C.end );
    printf( "Algorithm D: value: %d; start: %d; end: %d\n", D.value, D.start, D.end );

    A = intTester->algorithmA( test2, 15 );
    B = intTester->algorithmB( test2, 15 );
    C = intTester->algorithmC( test2, 0, 14 );
    D = intTester->algorithmD( test2, 15 );

    printf( "\nTest 2:\n" );

    printf( "Algorithm A: value: %d; start: %d; end: %d\n", A.value, A.start, A.end );
    printf( "Algorithm B: value: %d; start: %d; end: %d\n", B.value, B.start, B.end );
    printf( "Algorithm C: value: %d; start: %d; end: %d\n", C.value, C.start, C.end );
    printf( "Algorithm D: value: %d; start: %d; end: %d\n", D.value, D.start, D.end );

    A = intTester->algorithmA( test3, 12 );
    B = intTester->algorithmB( test3, 12 );
    C = intTester->algorithmC( test3, 0, 11 );
    D = intTester->algorithmD( test3, 12 );

    printf( "\nTest 3:\n" );

    printf( "Algorithm A: value: %d; start: %d; end: %d\n", A.value, A.start, A.end );
    printf( "Algorithm B: value: %d; start: %d; end: %d\n", B.value, B.start, B.end );
    printf( "Algorithm C: value: %d; start: %d; end: %d\n", C.value, C.start, C.end );
    printf( "Algorithm D: value: %d; start: %d; end: %d\n", D.value, D.start, D.end );

    A = intTester->algorithmA( test4, 10 );
    B = intTester->algorithmB( test4, 10 );
    C = intTester->algorithmC( test4, 0, 9 );
    D = intTester->algorithmD( test4, 10 );

    printf( "\nTest 4:\n" );

    printf( "Algorithm A: value: %d; start: %d; end: %d\n", A.value, A.start, A.end );
    printf( "Algorithm B: value: %d; start: %d; end: %d\n", B.value, B.start, B.end );
    printf( "Algorithm C: value: %d; start: %d; end: %d\n", C.value, C.start, C.end );
    printf( "Algorithm D: value: %d; start: %d; end: %d\n", D.value, D.start, D.end );

    A = intTester->algorithmA( test5, 9 );
    B = intTester->algorithmB( test5, 9 );
    C = intTester->algorithmC( test5, 0, 8 );
    D = intTester->algorithmD( test5, 9 );

    printf( "\nTest 5:\n" );

    printf( "Algorithm A: value: %d; start: %d; end: %d\n", A.value, A.start, A.end );
    printf( "Algorithm B: value: %d; start: %d; end: %d\n", B.value, B.start, B.end );
    printf( "Algorithm C: value: %d; start: %d; end: %d\n", C.value, C.start, C.end );
    printf( "Algorithm D: value: %d; start: %d; end: %d\n", D.value, D.start, D.end );

    A = intTester->algorithmA( test6, 10 );
    B = intTester->algorithmB( test6, 10 );
    C = intTester->algorithmC( test6, 0, 9 );
    D = intTester->algorithmD( test6, 10 );

    printf( "\nTest 6:\n" );

    printf( "Algorithm A: value: %d; start: %d; end: %d\n", A.value, A.start, A.end );
    printf( "Algorithm B: value: %d; start: %d; end: %d\n", B.value, B.start, B.end );
    printf( "Algorithm C: value: %d; start: %d; end: %d\n", C.value, C.start, C.end );
    printf( "Algorithm D: value: %d; start: %d; end: %d\n", D.value, D.start, D.end );

    A = intTester->algorithmA( test7, 9 );
    B = intTester->algorithmB( test7, 9 );
    C = intTester->algorithmC( test7, 0, 8 );
    D = intTester->algorithmD( test7, 9 );

    printf( "\nTest 7:\n" );

    printf( "Algorithm A: value: %d; start: %d; end: %d\n", A.value, A.start, A.end );
    printf( "Algorithm B: value: %d; start: %d; end: %d\n", B.value, B.start, B.end );
    printf( "Algorithm C: value: %d; start: %d; end: %d\n", C.value, C.start, C.end );
    printf( "Algorithm D: value: %d; start: %d; end: %d\n", D.value, D.start, D.end );

    Result<float> Af = floatTester->algorithmA( test8, 4 );
    Result<float> Bf = floatTester->algorithmB( test8, 4 );
    Result<float> Cf = floatTester->algorithmC( test8, 0, 3 );
    Result<float> Df = floatTester->algorithmD( test8, 4 );

    printf( "\nTest 8:\n" );

    printf( "Algorithm A: value: %.2f; start: %d; end: %d\n", Af.value, Af.start, Af.end );
    printf( "Algorithm B: value: %.2f; start: %d; end: %d\n", Bf.value, Bf.start, Bf.end );
    printf( "Algorithm C: value: %.2f; start: %d; end: %d\n", Cf.value, Cf.start, Cf.end );
    printf( "Algorithm D: value: %.2f; start: %d; end: %d\n", Df.value, Df.start, Df.end );


    A = intTester->algorithmA( test9, 3 );
    B = intTester->algorithmB( test9, 3 );
    C = intTester->algorithmC( test9, 0, 2 );
    D = intTester->algorithmD( test9, 3 );

    printf( "\nTest 9:\n" );

    printf( "Algorithm A: value: %d; start: %d; end: %d\n", A.value, A.start, A.end );
    printf( "Algorithm B: value: %d; start: %d; end: %d\n", B.value, B.start, B.end );
    printf( "Algorithm C: value: %d; start: %d; end: %d\n", C.value, C.start, C.end );
    printf( "Algorithm D: value: %d; start: %d; end: %d\n", D.value, D.start, D.end );
}

// the main function
int main ( ) {

    test( );
    return 0;

}
