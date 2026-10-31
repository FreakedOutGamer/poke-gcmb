#include "global.h"
#include "joybus.h"

#define COUPON_RECORD_SIZE 244

struct CouponLinkData
{
    u32 header;
    u8 filler_04[3];
    u8 transferCount;
    u8 filler_08[0x1C];
    u8 records[0x60D];
    u8 recordCount;
    u8 transferComplete;
};

extern volatile struct CouponLinkData gCouponLinkData;
extern u8 gCouponTransferBuffer[];

#define gCouponLinkDataViaBuffer \
    (*(volatile struct CouponLinkData *)(gCouponTransferBuffer - 0x5DC))

void ReceiveMonData(u32 value)
{
    u32 header;

    switch (gJoyBusState.state)
    {
    case 0:
        ((u32 *)gJoyBusState.buffer)[gJoyBusState.transferBytes / 4] = value;
        gJoyBusState.transferBytes += 4;
        if (gJoyBusState.transferBytes >= gJoyBusState.transferSize)
        {
            u8 *records;
            u32 mask;
            s32 i;
            volatile u8 *recordCount;

            header = (gCouponLinkData.header << 8) >> 8;
            gCouponLinkData.recordCount = 0;
            records = (u8 *)gCouponLinkData.records;
            mask = 0xF;
            recordCount = &gCouponLinkData.recordCount;

            for (i = 5; i >= 0; i--)
            {
                if ((header & mask) != 0xF)
                {
                    u32 count = *recordCount + 1;
                    (void)*recordCount;
                    *recordCount = count;
                }
                header >>= 4;
            }

            gJoyBusState.transferBytes = 0;
            gJoyBusState.buffer = (void *)records;
            gJoyBusState.transferSize = ((volatile u8 *)records)[0x60D]
                                        * COUPON_RECORD_SIZE;
            gJoyBusState.state++;
        }
        break;
    case 1:
        ((u32 *)gJoyBusState.buffer)[gJoyBusState.transferBytes / 4] = value;
        gJoyBusState.transferBytes += 4;
        if (gJoyBusState.transferBytes >= gJoyBusState.transferSize)
        {
            gJoyBusState.transferBytes = 0;
            gJoyBusState.buffer = (void *)gCouponTransferBuffer;
            gJoyBusState.transferSize = gCouponLinkDataViaBuffer.transferCount * 12;
            gJoyBusState.state++;
        }
        break;
    case 2:
        ((u32 *)gJoyBusState.buffer)[gJoyBusState.transferBytes / 4] = value;
        gJoyBusState.transferBytes += 4;
        if (gJoyBusState.transferBytes >= gJoyBusState.transferSize)
        {
            gJoyBusState.command = 0;
            gCouponLinkData.transferComplete = 1;
        }
        break;
    }
}
