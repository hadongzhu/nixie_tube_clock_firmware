/**
 * Generic FIFO buffer Implementation.
 *
 * Source code for the implementation of a FIFO buffer. The fifo can contain any
 * type of data, including but not limited to: integers, floats, structures,
 * arrays, etc.
 *
 * This data structure can be used on a variety of situations, for example:
 * It can be used with microcontroller peripherals for buffering, as a message
 * or event queue, can be used for task synchronization or comunications
 * and more.
 */

#ifndef FIFO_H
#define FIFO_H

#ifdef __cplusplus
extern "C" {
#endif
/*-------------------------------------------------------------*
 *		Includes and dependencies			*
 *-------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "compiler.h"

/*-------------------------------------------------------------*
 *		Macros and definitions				*
 *-------------------------------------------------------------*/

/*-------------------------------------------------------------*
 *		Typedefs enums & structs			*
 *-------------------------------------------------------------*/

/**
 * This structure holds the data that denfines a fifo buffer, and pointers
 * to access data on the buffer
 */
struct fifo_descriptor {
    /**
     * Start address on memory for item space.
     */
    void *item_space;
    /**
     * Size in bytes for each element in the buffer.
     */
    size_t item_size;
    /**
     * Size in bytes of the whole buffer.
     */
    size_t item_space_size;
    /**
     * Memory offset from which we will read data.
     */
    volatile uint32_t read_offset;
    /**
     * Memory offset where data will be written.
     */
    volatile uint32_t write_offset;
    /**
    ¨* Number of bytes have not been used currently.
     */
    volatile size_t item_space_free_size;
};

/**
 * Defines the side of the buffer to perform some actions
 */
typedef enum {
    E_FIFO_FRONT,
    E_FIFO_BACK,
} fifo_side_t;

/**
 * Definition for the fifo_t type, which is a pointer to a fifo_data struct
 */
typedef struct fifo_descriptor *fifo_t;

/*-------------------------------------------------------------*
 *		Function prototypes				*
 *-------------------------------------------------------------*/

__STATIC_INLINE bool fifo_is_full(fifo_t fifo)
{
    return (fifo->item_space_free_size == 0UL);
}

__STATIC_INLINE bool fifo_is_empty(fifo_t fifo)
{
    return (fifo->item_space_free_size == fifo->item_space_size);
}

__STATIC_INLINE size_t fifo_used_size(fifo_t fifo)
{
    return (fifo->item_space_size - fifo->item_space_free_size);
}

__STATIC_INLINE size_t fifo_free_size(fifo_t fifo)
{
    return (fifo->item_space_free_size);
}

#include "compiler.h"

__STATIC_INLINE void fifo_protect_atomic_operation_start(void)
{
    disable_irq();
}

__STATIC_INLINE void fifo_protect_atomic_operation_end(void)
{
    enable_irq();
}

/**
 * @brief Creates a FIFO using dynamic memory
 *
 * This function is used to create a buffer, it allocates memory for a buffer of
 * the requested size plus the size of the structure that contains the
 * information requeried by other API functions to access that buffer.
 *
 * @param count The number of elements the buffer should be able to store
 * @param size The size in bytes for each element to be stored in the buffer
 *
 * @return If a buffer is succesfully created, returns a pointer to the
 * structure that contains the buffer information (fifo_t). NULL is returned if
 * something fails.
 */
fifo_t fifo_create(uint32_t count, size_t size);

/**
 * @brief Creates a statically allocated FIFO buffer
 *
 * This function is similar to fifo_create() but it difers from the last because
 * the memory allocation is static. A pointer to the buffer space and a pointer
 * to the control structure are required, both of them shoud be allocated
 * statically by the programmer.
 *
 * @param fifo The pointer to the structure holding FIFO data
 * @param buf Pointer to the memory used to store actual fifo items
 * @param count The number of items to store on the FIFO
 * @param size The size in bytes of each item on the buffer
 *
 * @return This function performs some basic validation on the parameters
 * passed, if something is wrong with them,it will return NULL.
 */
fifo_t fifo_create_static(fifo_t fifo, void *buf, uint32_t count, size_t size);

/**
 * @brief Adds one item to the FIFO buffer
 *
 * This function writes an item to the fifo buffer back. This function affects
 * the write pointer and the stored items counter.
 *
 * The number of bytes to be copied to the fifo buffer was defined when the
 * fifo buffer was created with the function fifo_create() (size parameter).
 *
 * @param fifo Pointer to a fifo_descriptor structure.
 * @param item Pointer to a location that holds data to be written to the fifo
 * buffer.
 *
 * @return  Returns true if there is space in the FIFO to add the item. If the
 * buffer is full and no data can be copied it returns false.
 */
bool fifo_add(fifo_t fifo, const void *item);

/**
 * @brief Obtains one item from the FIFO buffer.
 *
 * This function reads an item from the fifo buffer front. This function affects
 * the read pointer and the stored items counter.
 *
 * The number of bytes to be copied to the provided buffer was defined when the
 * fifo buffer was created with the function fifo_create() (size parameter).
 *
 * @param fifo Pointer to a fifo_descriptor structure.
 * @param item Pointer to a location to hold the data read from the fifo buffer,
 * this buffer should be sized appropiately to accomodate the data.
 *
 * @return  Returns true if there is data available on the fifo buffer to be
 * copied, if the buffer is empty and no data can be read this returns false.
 */
bool fifo_get(fifo_t fifo, void *item);

/**
 * @brief Checks if the FIFO is full.
 *
 * Check if it can accept one item at least.
 *
 * @param fifo Pointer to a fifo_descriptor structure.
 *
 * @return This function returns true if the buffer is full, false otherwise.
 */
bool fifo_is_full(fifo_t fifo);

/**
 * @brief Checks if the FIFO is empty.
 *
 * Check if the buffer has no data stored in it.
 *
 * @param fifo Pointer to a fifo_descriptor structure.
 *
 * @return This function returns true if the buffer is empty, false otherwise.
 */
bool fifo_is_empty(fifo_t fifo);

/**
 * @brief Discard data from the buffer.
 *
 * This function discards data from the back or the front side of the buffer,
 * the side and the ammount of discarded data depends on the parameters passed.
 *
 * @param fifo Pointer to a fifo_descriptor structure.
 * @param count The number of elements to discard from the buffer.
 * @param side Defines if data should be discarted from the front or back side
 * of the buffer.
 *
 * @return Returns true if the data was discarted, false if not.
 */
bool fifo_discard(fifo_t fifo, uint32_t count, fifo_side_t side);

uint32_t fifo_get_buf(fifo_t fifo, void *item_buf, uint32_t item_num);
uint32_t fifo_add_buf(fifo_t fifo, void *item_buf, uint32_t item_num);

#ifdef __cplusplus
}
#endif

#endif
// End of Header file
