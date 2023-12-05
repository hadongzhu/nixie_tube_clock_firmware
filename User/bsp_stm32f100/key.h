/**
 ******************************************************************************
 * @file    key.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-10-09
 * @brief   Driver of keys.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __KEY_H
#define __KEY_H

#ifdef __cplusplus
#include <array>
#include <cstddef>
#include <cstdint>
#else
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#endif

#ifdef __cplusplus
namespace key {
namespace impl {
#endif

/**
 * @brief  Key action type definition.
 */
enum key_action
{
    KEY_ACTION_PUSH,
    KEY_ACTION_POP,
    KEY_ACTION_LONG_PRESS,
    KEY_ACTION_LONG_PRESS_REPEAT,
    KEY_ACTION_POP_AFTER_LONG_PRESS,
    KEY_ACTION_NUMBER,
};

/**
 * @brief  Key state type definition.
 */
enum key_state
{
    KEY_STATE_FREE,
    KEY_STATE_WAIT_FOR_LONG_PRESS,
    KEY_STATE_WAIT_FOR_REPEAT,
    KEY_STATE_WAIT_FOR_REPEAT_COOL_DOWN,
};

#ifdef __cplusplus
} // namespace key
} // namespace utils

namespace key {
enum class action : uint8_t
{
    push = key::impl::KEY_ACTION_PUSH,
    pop = key::impl::KEY_ACTION_POP,
    long_press = key::impl::KEY_ACTION_LONG_PRESS,
    long_press_repeat = key::impl::KEY_ACTION_LONG_PRESS_REPEAT,
    pop_after_long_press = key::impl::KEY_ACTION_POP_AFTER_LONG_PRESS,
};

static constexpr std::size_t action_number = key::impl::KEY_ACTION_NUMBER;

enum class state : uint8_t
{
    free = key::impl::KEY_STATE_FREE,
    wait_for_long_press = key::impl::KEY_STATE_WAIT_FOR_LONG_PRESS,
    wait_for_repeat = key::impl::KEY_STATE_WAIT_FOR_REPEAT,
    wait_for_repeat_cool_down = key::impl::KEY_STATE_WAIT_FOR_REPEAT_COOL_DOWN,
};

} // namespace key
#endif

#ifdef __cplusplus
using key_action = key::action;
using key_state = key::state;
#else
typedef uint8_t key_action;
typedef uint8_t key_state;
#endif

#ifdef __cplusplus
namespace key {
namespace impl_struct {
#endif

/**
 * @brief  Key event type definition.
 */
struct key_event
{
    uint8_t index;
    key_action action;
};

/**
 * @brief  Key entity definition.
 */
struct key_entity
{
    /* the pointer to a function that return 1 if the key is pressed and 0
     * on the contrary */
    bool (*is_key_active)(void);
    /* the counter of the filter */
    uint16_t filter_count;
    /* the counter of press time */
    uint16_t press_count;
    /* the limit of long press time */
    uint16_t long_press_time;
    /* time before repeat */
    uint16_t repeat_cool_down_time;
    /* the limit of repeat press time */
    uint16_t repeat_interval_time;
    /* the state of the key */
    key_state state;
};

#ifdef __cplusplus
} // namespace key
} // namespace utils

namespace key {
using event = key::impl_struct::key_event;
using entity = key::impl_struct::key_entity;

class controller
{
  public:
    constexpr static std::size_t key_number = 4U;
    constexpr static std::size_t filter_time = 5U;
    constexpr static std::size_t default_long_press_time = 100U;
    constexpr static std::size_t default_repeat_cool_down_time = 10U;
    constexpr static std::size_t default_repeat_interval_time = 10U;

  private:
    std::array<entity, key_number> keys;
    void (*fifo_add_event)(key::event event);
    bool (*fifo_get_event)(key::event &event);

  public:
    controller() = delete;
    controller(std::array<entity, key_number> keys,
               void (*fifo_add_event)(key::event event),
               bool (*fifo_get_event)(key::event &event))
        : keys(keys), fifo_add_event(fifo_add_event),
          fifo_get_event(fifo_get_event){};
    controller(std::array<bool (*)(void), key_number> is_key_active,
               void (*fifo_add_event)(key::event event),
               bool (*fifo_get_event)(key::event &event))
        : fifo_add_event(fifo_add_event), fifo_get_event(fifo_get_event)
    {
        for (std::size_t i = 0; i < key_number; i++)
        {
            keys[i]
                = entity{.is_key_active = nullptr,
                         .filter_count = 0U,
                         .press_count = 0,
                         .long_press_time = default_long_press_time,
                         .repeat_cool_down_time = default_repeat_cool_down_time,
                         .repeat_interval_time = default_repeat_interval_time,
                         .state = key::state::free};
            keys[i].is_key_active = is_key_active[i];
        }
    };

    void run();
    bool get_event(key::event &event)
    {
        return fifo_get_event(event);
    };
};
} // namespace key
#endif

#ifdef __cplusplus
using key_event = key::event;
using key_entity = key::entity;
using key_controller = key::controller;
extern key::controller key_controller_entity;
#else
typedef struct key_event key_event;
typedef struct key_entity key_entity;
typedef struct key_controller key_controller;
extern void key_controller_entity;
#endif

#ifdef __cplusplus
extern "C" {
#endif
void key_run(key_controller *controller);
#ifdef __cplusplus
}
#endif

#endif /* __KEY_H */
