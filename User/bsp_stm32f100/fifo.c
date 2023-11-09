#include "fifo.h"

/*-------------------------------------------------------------*
 *		Public API implementation			*
 *-------------------------------------------------------------*/
fifo_t fifo_create(uint32_t item_space_size, size_t item_size)
{
    fifo_t fifo;
    if (item_space_size > 0)
    {
        fifo = (struct fifo_descriptor *)malloc(sizeof(struct fifo_descriptor));
        if (fifo != NULL)
        {
            // Calculate the size in bytes of the buffer
            size_t item_space_size_bytes = item_space_size * item_size;
            // Try to allocate space for the buffer data
            fifo->item_space = malloc(item_space_size_bytes);
            if (fifo->item_space != NULL)
            {
                // Initialize structure members
                fifo->item_size = item_size;
                fifo->item_space_size = item_space_size_bytes;
                fifo->read_offset = 0;
                fifo->write_offset = 0;
                fifo->item_space_free_size = fifo->item_space_size;
                // Return the pointer to fifo_descriptor structure
                return fifo;
            }
            else
            {
                // Cannot allocate space for items, free struct resources
                free(fifo);
            }
        }
    }
    // Return NULL if something fails
    return NULL;
}

fifo_t fifo_create_static(fifo_t fifo, void *buf, uint32_t item_space_size,
                          size_t item_size)
{
    // Sanity check for memory and element sizes
    if (buf != NULL && fifo != NULL && item_space_size != 0)
    {
        fifo->item_space = buf;
        fifo->item_size = item_size;
        fifo->item_space_size = item_space_size * item_size;
        fifo->read_offset = 0;
        fifo->write_offset = 0;
        fifo->item_space_free_size = fifo->item_space_size;
        return fifo;
    }
    return NULL;
}

bool fifo_add(fifo_t fifo, const void *item)
{
    if (!fifo_is_full(fifo))
    {
        memcpy((uint8_t *)fifo->item_space + fifo->write_offset, item,
               fifo->item_size);
        fifo_protect_atomic_operation_start();
        fifo->item_space_free_size -= fifo->item_size;
        fifo->write_offset += fifo->item_size;
        if (fifo->write_offset >= fifo->item_space_size)
        {
            fifo->write_offset = 0;
        }
        fifo_protect_atomic_operation_end();
        return true;
    }
    else
    {
        return false;
    }
}

bool fifo_get(fifo_t fifo, void *item)
{
    if (!fifo_is_empty(fifo))
    {
        memcpy(item, (uint8_t *)fifo->item_space + fifo->read_offset,
               fifo->item_size);
        fifo_protect_atomic_operation_start();
        fifo->item_space_free_size += fifo->item_size;
        fifo->read_offset += fifo->item_size;
        if (fifo->read_offset >= fifo->item_space_size)
        {
            fifo->read_offset = 0;
        }
        fifo_protect_atomic_operation_end();
        return true;
    }
    else
    {
        return false;
    }
}

uint32_t fifo_add_buf(fifo_t fifo, void *item_buf, uint32_t item_num)
{
    size_t free_size = 0L;
    size_t item_buff_size = item_num * fifo->item_size;
    size_t first_part_size;
    if ((fifo != NULL) && (item_buf != NULL) && (item_num != 0))
    {
        free_size = fifo_free_size(fifo);
        if (free_size <= item_buff_size)
        {
            item_buff_size = free_size;
        }
        if (fifo->write_offset + item_buff_size <= fifo->item_space_size)
        {
            (void)memcpy((uint8_t *)fifo->item_space + fifo->write_offset,
                         item_buf, item_buff_size);
        }
        else
        {
            first_part_size = fifo->item_space_size - fifo->write_offset;
            (void)memcpy((uint8_t *)fifo->item_space + fifo->write_offset,
                         item_buf, first_part_size);
            (void)memcpy(fifo->item_space,
                         (uint8_t *)item_buf + first_part_size,
                         item_buff_size - first_part_size);
        }

        fifo_protect_atomic_operation_start();
        fifo->item_space_free_size -= item_buff_size;
        fifo->write_offset += item_buff_size;
        if (fifo->write_offset >= fifo->item_space_size)
        {
            fifo->write_offset %= fifo->item_space_size;
        }
        fifo_protect_atomic_operation_end();
    }
    return item_buff_size / fifo->item_size;
}

uint32_t fifo_get_buf(fifo_t fifo, void *item_buf, uint32_t item_num)
{
    size_t used_size = 0L;
    size_t item_buff_size = item_num * fifo->item_size;
    size_t first_part_size;
    if ((fifo != NULL) && (item_buf != NULL) && (item_num != 0))
    {
        used_size = fifo_used_size(fifo);
        if (used_size <= item_buff_size)
        {
            item_buff_size = used_size;
        }
        if (fifo->read_offset + item_buff_size <= fifo->item_space_size)
        {
            (void)memcpy(item_buf,
                         (uint8_t *)fifo->item_space + fifo->read_offset,
                         item_buff_size);
        }
        else
        {
            first_part_size = fifo->item_space_size - fifo->read_offset;
            (void)memcpy(item_buf,
                         (uint8_t *)fifo->item_space + fifo->read_offset,
                         first_part_size);
            (void)memcpy((uint8_t *)item_buf + first_part_size,
                         fifo->item_space, item_buff_size - first_part_size);
        }

        fifo_protect_atomic_operation_start();
        fifo->item_space_free_size += item_buff_size;
        fifo->read_offset += item_buff_size;
        if (fifo->read_offset >= fifo->item_space_size)
        {
            fifo->read_offset %= fifo->item_space_size;
        }
        fifo_protect_atomic_operation_end();
    }
    return item_buff_size / fifo->item_size;
}

bool fifo_discard(fifo_t fifo, uint32_t count, fifo_side_t side)
{
    uint32_t step;
    step = fifo->item_size
           * count; // Compute byte size of elements to be deleted
    if (step <= fifo->item_space_size
                    - fifo->item_space_free_size) // Check if we can remove the
                                                  // requested ammount of data
    {
        if (side == E_FIFO_FRONT)
        {
            fifo->read_offset
                = (fifo->read_offset + step)
                  % fifo->item_space_size; // Increase read pointer n elements
        }
        else if (side == E_FIFO_BACK)
        {
            fifo->write_offset
                = (fifo->write_offset - step)
                  % fifo->item_space_size; // Decrease write pointer n elements
        }
        fifo->item_space_free_size += step;
        return true;
    }
    return false;
}
