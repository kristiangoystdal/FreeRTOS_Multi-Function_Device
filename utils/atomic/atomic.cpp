
#include "atomic.hpp"
#include <stdio.h>

namespace atomic {
  // Define a class within the namespace
  template <typename T>
  class Atomic {
  public:
    // Constructor
    Atomic(T value) : value(value) {
      xMutex = xSemaphoreCreateMutex();
      if(xMutex == NULL) {
        printf("Critical error when creating Mutex!");
      }
    }

    // Member function
    void set(T value) {
      xSemaphoreTake(xMutex, portMAX_DELAY);
      this->value = value;
      xSemaphoreGive(xMutex);
    }

    T get(void) {
      xSemaphoreTake(xMutex, portMAX_DELAY);
      T val = this->value;
      xSemaphoreGive(xMutex);
      return val;
    }

  private:
    SemaphoreHandle_t xMutex;
    T value;
  };
}