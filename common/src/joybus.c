#include "global.h"
#include "joybus.h"

// "PKEG": the word the client and the GameCube exchange to recognize each
// other.
#define JOYBUS_HANDSHAKE_WORD 0x47454B50

// Two of JOYSTAT's general-purpose flags, which the GameCube reads.
#define JOYSTAT_HANDSHAKE_SENT (1 << 5)
#define JOYSTAT_RESULT_SENT    (1 << 4)

// Runs the JoyBus handshake with the GameCube. Returns 0 once it completes,
// or 1-3 if the GameCube breaks the expected sequence.
u8 JoyBusHandshake(void)
{
    u8 state = 0;
    u16 joystat;
    u16 joycnt;
    u32 recv;

    REG_JOYCNT |= JOYCNT_RESET | JOYCNT_RECV | JOYCNT_SEND;
    REG_JOYSTAT = 0;

    for (;;)
    {
        joystat = REG_JOYSTAT;
        joycnt = REG_JOYCNT;
        switch (state)
        {
        case 0:
            // Wait for the GameCube to reset the link, then offer our word.
            if (joycnt & JOYCNT_RESET)
            {
                recv = REG_JOY_RECV;
                REG_JOY_RECV;
                REG_JOY_TRANS = JOYBUS_HANDSHAKE_WORD;
                REG_JOYSTAT |= JOYSTAT_HANDSHAKE_SENT;
                REG_JOYCNT |= JOYCNT_RESET;
                state++;
            }
            break;
        case 1:
            if (joycnt & JOYCNT_SEND)
            {
                REG_JOYCNT |= JOYCNT_SEND;
                state++;
            }
            else if (joycnt & JOYCNT_RESET)
            {
                state = 0;
            }
            else if (joycnt & JOYCNT_RECV)
            {
                return 1;
            }
            break;
        case 2:
            if ((joycnt & JOYCNT_RECV) || (joystat & JOYCNT_RECV))
            {
                if (joycnt & JOYCNT_RECV)
                    REG_JOYCNT |= JOYCNT_RECV;
                state++;
            }
            else if (joycnt & JOYCNT_RESET)
                state = 0;
            else if (joycnt & JOYCNT_SEND)
                return 2;
            break;
        case 3:
            // The GameCube must answer with the same word.
            recv = REG_JOY_RECV;
            REG_JOY_RECV;
            if (recv != JOYBUS_HANDSHAKE_WORD)
                return 3;
            REG_JOYSTAT |= JOYSTAT_HANDSHAKE_SENT | JOYSTAT_RESULT_SENT;
            state++;
            break;
        case 4:
            if (joycnt & (JOYCNT_RESET | JOYCNT_RECV | JOYCNT_SEND))
            {
                if (joycnt & JOYCNT_RESET)
                    REG_JOYCNT |= JOYCNT_RESET;
                if (joycnt & JOYCNT_SEND)
                    REG_JOYCNT |= JOYCNT_SEND;
                if (joycnt & JOYCNT_RECV)
                    REG_JOYCNT |= JOYCNT_RECV;
                REG_JOYSTAT &= ~(JOYSTAT_HANDSHAKE_SENT | JOYSTAT_RESULT_SENT);
                return 0;
            }
            break;
        }
        VBlankIntrWait();
    }
}

// Reports a result to the GameCube once. The flag stays set until the
// GameCube has read it.
void JoyBusSendResult(u32 result)
{
    if (!(REG_JOYSTAT & JOYSTAT_RESULT_SENT))
    {
        REG_JOY_TRANS = result;
        REG_JOYSTAT |= JOYSTAT_RESULT_SENT;
    }
}
