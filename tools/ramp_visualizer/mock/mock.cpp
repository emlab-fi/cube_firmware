#include <vector>
#include <cassert>
#include "stm32g4xx_hal.h"

void print_data(const auto& data) {
    bool is_first = true;
    std::cout << "[";
    for (const auto num : data) {
        if (!is_first) {
            std::cout << ", ";
        }
        std::cout << num;
        is_first = false;
    }
    std::cout << "]";
}

HAL_StatusTypeDef HAL_TIM_Base_Start(TIM_HandleTypeDef*) {return HAL_OK;}
HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef*, uint32_t) {return HAL_OK;}
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef*, uint32_t) {return HAL_OK;}
HAL_StatusTypeDef HAL_TIM_DMABurst_MultiWriteStart(TIM_HandleTypeDef*, uint32_t, uint32_t, uint32_t* dma_data, uint32_t, uint32_t dma_size) {
    assert(dma_data != nullptr); 
    uint16_t* original_data = reinterpret_cast<uint16_t*>(dma_data);
    std::vector<uint16_t> data(original_data, original_data + dma_size);
    
    print_data(data);
    
    return HAL_OK;
}
void HAL_GPIO_WritePin(GPIO_TypeDef*, uint16_t, GPIO_PinState) {}

namespace cube_hw
{
    void log_info(const char*, ...) {}
    void log_error(const char*, ...) {}
} // namespace cube_hw