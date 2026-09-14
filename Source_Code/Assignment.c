#include<stdint.h>

#define NO_KEY 0x0F  //four active low keys
#define KEY1 0x0E  // 0000 1110
#define KEY2 0x0D  // 0000 1101
#define KEY3 0x0B  // 0000 1011
#define KEY4 0x07  // 0000 0111

/*State variable bit fields
bits 0-3: key value, bit 4-6 : debounce counter, bit 7 : event flag
*/
#define KEY_MASK 0x0F
#define COUNT_MASK 0x70
#define EVENT_MASK 0x80

#define COUNT_SHIFT 4
#define DEBOUNCE_LIMIT 5  //number of identical press of same button

#define READ_KEYS() (NO_KEY) 

//GPIO abstraction 1 = released, 0 = pressed, PULL UP ckt

void check_switches(void) //non-blocking key scanner
{
    static uint8_t state = (NO_KEY | (DEBOUNCE_LIMIT <<COUNT_SHIFT)); //8 bit variable

    //detected a change in GPIO state
    if((READ_KEYS() & KEY_MASK) != (state & KEY_MASK)) 
    {
        //store the new key state and preserve event flag
        state = (READ_KEYS() & KEY_MASK) | (1U << COUNT_SHIFT) | (state & EVENT_MASK); 
        return;
    }

    //i/p is unchanged, increase the debounce counter
    if((state & COUNT_MASK) < (DEBOUNCE_LIMIT << COUNT_SHIFT))
    {
        state += (1U << COUNT_SHIFT);
        return;
    }

    //process only after input is stable
    if((state & EVENT_MASK) == 0)
    {
        switch(state & KEY_MASK)
        {
            case KEY1:
            //SW1 pressed
            state |= EVENT_MASK;
            break;

            case KEY2:
            //SW2 pressed
            state |= EVENT_MASK;
            break;

            case KEY3:
            //SW3 pressed
            state |= EVENT_MASK;
            break;

            case KEY4:
            //SW4 pressed
            state |= EVENT_MASK;
            break;

            default:
            //NO_KEY or invalid/multiple-key input
            break;

        }
    }

    //stable release allows the next press to generate an event
    if((state & KEY_MASK) == NO_KEY)
    {
        state &= (uint8_t) ~EVENT_MASK;
    }
}

int main()
{
    //call periodically from timer/schedular
    while(1)
    {
        check_switches();
    }
    return 0;
}
