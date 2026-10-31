#ifndef GUARD_LINK_BUFFERS_H
#define GUARD_LINK_BUFFERS_H

#include "joybus.h"

// Transfer buffers and flags follow the fixed state structure.
#define JOYBUS_EXTRA(offset) (*(vu8 *)((u8 *)&gJoyBusState + (offset)))

#define JOYBUS_XFER_DONE  0x2AA0 // Graphics transfer complete.

#define JOYBUS_CMD99_DONE 0x2AA1 // The 0x68-byte gift transfer is complete.

#define JOYBUS_CMD88_FLAG 0x2AA2 // Set by command 0x88; cleared by command 0x99.

#define JOYBUS_POLL_STATUS 0x2AA4 // Status byte returned by command 0x77.

#endif // GUARD_LINK_BUFFERS_H
