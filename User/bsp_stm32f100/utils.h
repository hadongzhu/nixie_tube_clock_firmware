/**
 ******************************************************************************
 * @file    utils.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-09-06
 * @brief   provide utils.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __UTILS_H
#define __UTILS_H

#ifdef __cplusplus
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#endif

#ifdef __cplusplus
namespace utils {
namespace impl {
#endif

enum utils_breath_direction
{
    BREATH_DIRECTION_UP = 1,
    BREATH_DIRECTION_DOWN = -1,
};

enum utils_twinkle_state
{
    TWINKLE_STATE_ON,
    TWINKLE_STATE_OFF,
};

#ifdef __cplusplus
} // namespace impl
} // namespace utils

namespace utils {
enum class breath_direction : int8_t
{
    up = impl::BREATH_DIRECTION_UP,
    down = impl::BREATH_DIRECTION_DOWN,
};

enum class twinkle_state : uint8_t
{
    on = impl::TWINKLE_STATE_ON,
    off = impl::TWINKLE_STATE_OFF,
};

uint32_t number_breath_calculate(int32_t number, uint32_t min, uint32_t max,
                                 uint32_t step,
                                 utils::breath_direction &direction);
int32_t number_change_with_limitation(int32_t origin_number, int32_t volume,
                                      int32_t min, int32_t max);

//template <typename T, std::size_t size>
//constexpr std::array<T, size> make_array(T const item, std::size_t const = size)
//{
//    std::array<T, size> ret;
//    std::fill(ret.begin(), ret.end(), item);
//    return ret;
//};

//namespace pin {
//enum class state : int8_t
//{
//    reset = false,
//    set = true,
//};

//class o
//{
//  private:
//    void (*set_pin)(const pin::state state) = nullptr;

//  public:
//    o(void (*set_pin)(pin::state state)) : set_pin(set_pin){};
//    o &operator=(const pin::state state)
//    {
//        set_pin(state);
//        return *this;
//    };
//};

//class i
//{
//  private:
//    pin::state (*get_pin)(void) = nullptr;

//  public:
//    i(pin::state (*get_pin)(void)) : get_pin(get_pin){};
//    pin::state operator()(void) const
//    {
//        return get_pin();
//    };
//};

//class io : public o, public i
//{
//  public:
//    io(void (*set_pin)(pin::state state), pin::state (*get_pin)(void))
//        : o(set_pin), i(get_pin){};
//};

//} // namespace pin

} // namespace utils

#endif

#ifdef __cplusplus
using utils_breath_direction = utils::breath_direction;
using utils_twinkle_state = utils::twinkle_state;
#else
typedef int8_t utils_breath_direction;
typedef uint8_t utils_twinkle_state;
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    uint32_t utils_number_breath_calculate(int32_t number, uint32_t min,
                                           uint32_t max, uint32_t step,
                                           utils_breath_direction *direction);

#ifdef __cplusplus
}
#endif

#endif /* __UTILS_H */
