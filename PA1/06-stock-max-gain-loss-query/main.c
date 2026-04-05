#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>

// Structure representing the derivation details
typedef struct {
    int derivation; // Profit or loss value
    int startTime;  // Start time index of the derivation
    int endTime;    // End time index of the derivation
} TDerivation;

// Dynamic array to store action prices
typedef struct {
    int *data;       // Pointer to the array of prices
    size_t size;     // Current number of elements in the array
    size_t capacity; // Current capacity of the array
} TActionPricesArray;

// Function to create a derivation structure
TDerivation makeDerivation(int derivation, int startTime, int endTime) {
    TDerivation res;
    res.derivation = derivation;
    res.startTime = startTime;
    res.endTime = endTime;
    return res;
}

// Function to initialize the dynamic array
TActionPricesArray initActionsArray() {
    TActionPricesArray array;
    array.data = (int*)malloc(sizeof(int)); // Allocate memory for the array
    array.size = 0;
    array.capacity = 1;
    return array;
}

// Function to free the memory used by the dynamic array
void destroyActionsArray(TActionPricesArray *array) {
    free(array->data);
}

// Function to add a new price to the dynamic array
void addActionPrice(TActionPricesArray *array, int price) {
    // Check if the array needs to grow
    if ((array->size + 1) > array->capacity) {
        array->capacity *= 2;
        array->data = (int*)realloc(array->data, array->capacity * sizeof(int)); // Expand allocated memory for the array
    }
    array->data[array->size++] = price; // Add the new price and increment current size
}

// Function to calculate and print the maximum profit and loss derivations
void printMaxDerivations(const TActionPricesArray* array, int startTime, int endTime) {
	// Initialize maximum derivations
    TDerivation maxProfit = makeDerivation(INT_MIN, 0, 0);
    TDerivation maxLoss = makeDerivation(INT_MAX, 0, 0);

	// Initialize the minimum/maximum price with the first element in the range
	// And set the starting time index for the minimum/maximum price
    int minPrice = array->data[startTime], minStartTime = startTime;
    int maxPrice = array->data[startTime], maxStartTime = startTime;

	// Iterate through the price array to find the maximum profit and loss derivations
    for (int i = startTime + 1; i <= endTime; ++i) {
        // Check for maximum profit
        if (array->data[i] - minPrice > maxProfit.derivation) {
            maxProfit = makeDerivation(array->data[i] - minPrice, minStartTime, i);
        }
        // Check for maximum loss
        if (array->data[i] - maxPrice < maxLoss.derivation) {
            maxLoss = makeDerivation(array->data[i] - maxPrice, maxStartTime, i);
        }
        // Update minimum price
        if (array->data[i] < minPrice) {
            minPrice = array->data[i];
            minStartTime = i;
        }
        // Update maximum price
        if (array->data[i] > maxPrice) {
            maxPrice = array->data[i];
            maxStartTime = i;
        }
    }

    // Print the results
    if (maxProfit.derivation > 0) {
        printf("Nejvyssi zisk: %d (%d - %d)\n", maxProfit.derivation, maxProfit.startTime, maxProfit.endTime);
    } else {
        printf("Nejvyssi zisk: N/A\n");
    }

    if (maxLoss.derivation < 0) {
        printf("Nejvyssi ztrata: %d (%d - %d)\n", -maxLoss.derivation, maxLoss.startTime, maxLoss.endTime);
    } else {
        printf("Nejvyssi ztrata: N/A\n");
    }
}

// Main function to handle user input and control program flow
int main() {
    TActionPricesArray actionPricesArray = initActionsArray(); // Initialize main array of prices

	// Initialize operation values
    int price, startTime, endTime;
    int errcode = 0;
    char operationType;

	printf("Ceny, hledani:\n");

	// Read and add prices to main array till EOF occures
    while (errcode != EOF) {
        errcode = scanf(" %c ", &operationType); // Read operation type and save returned code for further analyze
		
		if (errcode == EOF) break; // Exit on EOF

        // Handle invalid input
        if (errcode != 1 || (operationType != '+' && operationType != '?')) {
            printf("Nespravny vstup.\n");
            destroyActionsArray(&actionPricesArray);
            return EXIT_FAILURE;
        }

		// Perform operation depend on readed operation type
        if (operationType == '+') {
			// Read a new price and handle invalid input
            if (scanf("%d", &price) != 1 || price < 0) {
                printf("Nespravny vstup.\n");
                destroyActionsArray(&actionPricesArray);
                return EXIT_FAILURE;
            }
            addActionPrice(&actionPricesArray, price); // Add a readed new price
        } else if (operationType == '?') {
            // Read scanning interval and handle invalid input
            if (scanf("%d %d", &startTime, &endTime) != 2 || startTime < 0 || endTime >= (int)actionPricesArray.size || startTime > endTime) {
                printf("Nespravny vstup.\n");
                destroyActionsArray(&actionPricesArray);
                return EXIT_FAILURE;
            }
            printMaxDerivations(&actionPricesArray, startTime, endTime); // Scan maximum profit and loss in readed interval
        }
    }

    // Cleanup and exit
    destroyActionsArray(&actionPricesArray);
    return EXIT_SUCCESS;
}