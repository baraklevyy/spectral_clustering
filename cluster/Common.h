/*
 * Common.h
 *
 *  Created on: 8 Aug 2020
 *      Author: barak
 */

#ifndef COMMON_H_
#define COMMON_H_
#define IS_POSITIVE(X) ((X) > 0.00001)
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

 /*Creating Linked-list. Each cell contain the column index, data and next reference*/


typedef enum _Status {
	INVALID_STATUS_CODE = -1,
	SUCCESS_STATUS_CODE = 0,
	MALLOC_FAILED_CODE,
	NULL_ARGUMENT_CODE,
	FOPEN_FAILED_CODE,
	FREAD_FAILED_CODE,
	FWRITE_FAILED_CODE,
	INVALID_ARGUMENTS_CODE,
	ZERO_DIVISION_CODE,
	GROUP_NOT_DIVISIBLE_CODE,
	//COUNTER FOR ERRORS
	STATUS_CODE_COUNT
}Status, *PStatus;

void get_error_message(Status result);

#endif /* COMMON_H_ */
