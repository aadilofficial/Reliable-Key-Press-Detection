# Reliable Key Press Detection

A bare-metal C implementation for reliable detection of key presses from four GPIO-connected switches.

The design focuses on rejecting switch bounce and short-duration noise while ensuring that a valid key press generates only one event, even when the key is held down.

## Objective

The objective is to reliably detect key presses from four GPIO switches while:

- Rejecting mechanical switch bounce and short-duration noise
- Detecting only stable key presses
- Generating only one event for a continuous key press
- Supporting key release and subsequent re-press
- Avoiding blocking delays
- Using only one 8-bit state variable
- Keeping the implementation portable C

## Key Mapping

The switches are assumed to be active-low.

| Input | Key |
|------|-----|
| `0x0F` | No key pressed |
| `0x0E` | KEY1 |
| `0x0D` | KEY2 |
| `0x0B` | KEY3 |
| `0x07` | KEY4 |

Any other combination is treated as an invalid/multiple-key condition and is ignored.

## Debouncing Approach

Mechanical switches can produce multiple rapid transitions when pressed or released.

To avoid interpreting these transitions as multiple key presses, the implementation uses consecutive-sample validation.

A key state is considered stable only after receiving the same input for a predefined number of consecutive scans.

The current implementation uses:

```c
#define DEBOUNCE_LIMIT 5
