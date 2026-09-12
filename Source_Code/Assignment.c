#include<stdint.h>

#define NO_KEY 0x0F  //four active low keys

#define KEY1 0x0E  //1110
#define KEY2 0x0D  //1101
#define KEY3 0x0B  //1011
#define KEY4 0x07  //0111

/*State-byte fields */
#define KEY_MASK 0x0F
#define COUNT_MASK 0x70
#define EVENT_MASK 0x80
#define COUNT_SHIFT 4

#define DEBOUNCE_LIMIT 5  //number of identical press of same button

//GPIO abstraction 1 = released, 0 = pressed

#define GPIO_READ() (NO_KEY) //replace this macro with actual GPIO access

static int is_valid_key(uint8_t key) //Only one valid key
{
    return (key == KEY1 || key == KEY2 || key == KEY3 || key == KEY4);
}

static void key_event(uint8_t key) //action for confirmed key pressed
{
    switch(key)
    {
        case KEY1:
        //task for SW1
        break;

        case KEY2:
        //task for SW2
        break;

        case KEY3:
        //task for SW3
        break;

        case KEY4:
        //task for SW4
        break;

        default:
        break;
    }
}

static void key_scan(void) //non-blocking key scanner
{
    static uint8_t state = NO_KEY;

    //new i/p detected store it and start debounce counting
    if((GPIO_READ() & KEY_MASK) != (state & KEY_MASK)) 
    {
        state = (GPIO_READ() & KEY_MASK); 
        return;
    }

    //i/p is unchanged, increase the debounce counter
    if((state & COUNT_MASK) < (DEBOUNCE_LIMIT << COUNT_SHIFT))
    {
        state += (1U << COUNT_SHIFT);
        return;
    }

    //i/p is stable long enough
    if(is_valid_key(state & KEY_MASK))
    {
        //generate only one event for one press
        if((state & EVENT_MASK) == 0)
        {
            state |= EVENT_MASK;
            key_event(state & KEY_MASK);
        }
    }

    //no key is pressed and clear event flag for new event
    else
    {
        state &= ~EVENT_MASK;
    }
}

int main()
{
    while(1)
    {
        key_scan();
    }
    return 0;
}
