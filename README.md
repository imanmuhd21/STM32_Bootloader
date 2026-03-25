# STM32 Bare-Metal Bootloader

Custom bare-metal bootloader for STM32 Cortex-M microcontrollers implementing flash partitioning, vector table relocation, stack pointer reconfiguration and a safe jump-to-application mechanism.

The bootloader is compiled as a separate binary from the main firmware, forming the foundation for reliable firmware update (OTW / OTA) systems.

---

## 🚀 Features

* Custom flash memory partitioning
* Safe application handover mechanism
* Bare-metal peripheral drivers (Flash / UART / SysTick)
* Interrupt-driven UART with ring buffer
* Reliable OTW firmware update protocol
* Dual UART debug + data architecture
* Transport-agnostic OTA design (future Bluetooth support)

---

## 📍 Flash Memory Layout

Total Internal Flash Size: **512 KB**

| Region      | Address Range             | Size     | Usage                     |
| ----------- | ------------------------- | -------- | ------------------------- |
| Bootloader  | `0x08000000 – 0x0800BFFF` | 48 KB    | Boot firmware             |
| Reserved    | `0x0800C000 – 0x0801FFFF` | Reserved | Future metadata / staging |
| Application | `0x08020000 – 0x0803FFFF` | 128 KB   | User firmware             |

---

## 🧠 Boot Execution Flow

After reset or power-up:

1. CPU loads initial MSP from `0x08000000`
2. CPU jumps to Bootloader Reset Handler at `0x08000004`
3. Bootloader startup performs:

   * `.data` initialization
   * `.bss` clearing
   * System initialization
4. Bootloader `main()` decides whether to:

   * Enter firmware update mode
   * Or jump to application

---

## 🔁 Safe Jump-to-Application

```c
__set_MSP(*(uint32_t*)0x08020000);

void (*app_reset_handler)(void);
app_reset_handler = (void*)(*(uint32_t*)0x08020004);

app_reset_handler();
```

Without MSP reconfiguration, the application would use the bootloader stack and cause undefined behaviour.

---

## 🧩 VTOR Relocation (Application Side)

```c
SCB->VTOR = 0x08020000;
```

Ensures NVIC fetches ISR handlers from the application vector table.

Configured inside `SystemInit()`.

---

## 📜 Linker Script Configuration

Bootloader:

```ld
FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 48K
```

Application:

```ld
FLASH (rx) : ORIGIN = 0x08020000, LENGTH = 128K
```

---

## 🎮 Boot Mode Selection (Active-LOW User Button)

* PC13 configured with internal pull-up
* Button connected to GND

| Button      | PC13 Level | Action              |
| ----------- | ---------- | ------------------- |
| Not pressed | HIGH       | Jump to application |
| Pressed     | LOW        | Enter OTA update    |

Bootloader checks button state for **first 3 seconds after reset**.

---

## ⏱ Custom SysTick Driver

System Clock: **16 MHz**

```c
SysTick->LOAD = (16000000 / 1000) - 1;
```

ISR:

```c
uwTick++;
```

Time retrieval:

```c
uint32_t Get_Tick(void);
```

---

## 📡 Dual UART Architecture

| UART  | Purpose                    |
| ----- | -------------------------- |
| UART2 | Firmware packet reception  |
| UART6 | Debug logging (`myprintf`) |

Debug output routed via CH340 USB-UART bridge.

---

## 📦 OTA / OTW Update Layer

Bootloader invokes:

```c
ota_update_start();
```

Handles firmware reception and flash programming.

---

## 📄 Packet Protocol

```
SOF | PACKET TYPE | DATA LEN | DATA | CRC | EOF
1B    1B            2B         nB     4B    1B
```

Markers:

```
SOF = 0xAA
EOF = 0xBB
```

---

## 🔄 OTA State Machine

```
IDLE → START → HEADER → DATA → END → IDLE
```

Abort command can terminate update from any state.

### State Behaviour

**START**

* Validate CMD_START
* Transition → HEADER

**HEADER**

* Receive firmware metadata
* Transition → DATA

**DATA**

* Write chunks into application flash region
* When firmware size reached → END

**END**

* Validate CMD_END
* Transition → IDLE

ACK / NACK sent after each state.

---

## ⚡ Interrupt-Driven UART with Ring Buffer

* RX handled via interrupt
* Ring buffer prevents blocking

Benefits:

* Deterministic timing
* High-speed firmware streaming
* Reliable packet handling

---

## 🏗 System Architecture

```
PC Host Tool (UART / Future Bluetooth)
            │
            ▼
      STM32 Bootloader
  (OTA State Machine + Drivers)
            │
            ▼
      User Application
        (TTY Shell)
```

---

## 🔮 Future Enhancements

* Bluetooth OTA using BT-05
* Firmware authenticity validation
* Secure update mechanism
* Version rollback support

---

## 🎯 Project Purpose

Demonstrates advanced embedded firmware engineering skills:

* Cortex-M boot sequence control
* Flash memory architecture design
* Custom firmware update protocol
* Interrupt-driven buffering strategy
* Safe firmware handover mechanism
