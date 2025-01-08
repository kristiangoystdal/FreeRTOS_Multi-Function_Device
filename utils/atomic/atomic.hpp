
#ifndef __ATOMIC_H
#define __ATOMIC_H

#include "FreeRTOS.h"
#include "semphr.h"

namespace atomic {

  // Define a class within the namespace
  template <typename T>
  class Atomic {
  public:
    // Constructor
    Atomic(T value);

    // Member function
    void set(T value);
    T get(void);
  private:
    SemaphoreHandle_t xMutex;
    T value;
  };

} // atomic


#endif // __ATOMIC_H
