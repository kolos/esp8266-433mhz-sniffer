# ESP8266 433 MHz RF Sniffer & Decoder

ESP8266 433 MHz RF sniffer with burst filtering and bit‑level decode.  
Captures, validates, and prints raw pulses and decoded signals for RF remote analysis.

## ✨ Features
- Interrupt‑driven pulse capture for accurate timing
- Burst filtering with repeat validation to suppress noise
- Prints **raw pulse widths** and **decoded bitstream**
- Simple jitter tolerance for robust decoding

## 📜 Usage
1. Flash the sketch to your ESP8266.
2. Open the **Serial Monitor** at `115200 baud`.
3. Press buttons on your 433 MHz remote.
4. Output will show:
   - **RAW pulse widths** (comma‑separated)
   - **Decoded bitstream** (`0`, `1`, `_` for unknown)

Example output:
```
RAW: 10010,838,286,822,293,818,298,814,304,809,301,811,303,810,305,809,306,807,304,807,308,805,307,801,313,803,318,794,316,798,313,800,318,794,316,793,321,797,319,793,322,792,322,787,325,785,333,787,321,291,822,292,815,300,819,792,316,299,818,794,317,795,324,790,321,791,321,792,322,789,326,289,821,792,330,286,817,296,814,299,814,303
BITS: 101010101010101010101010101010101010101010101010010101100110101010101001100101010
```