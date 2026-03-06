#  🏓 tcping

> **TCP-based ping tool — measure round-trip time using a full TCP handshake, with QoS/TOS support.**

![Language](https://img.shields.io/badge/language-C-blue?style=flat-square)
![Version](https://img.shields.io/badge/version-1.0-green?style=flat-square)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS-lightgrey?style=flat-square)
![License](https://img.shields.io/badge/license-MIT-orange?style=flat-square)

---

## 📖 Description

**tcping** is a low-level network tool written in C that replicates the behavior of `ping`, but at the **TCP layer** instead of ICMP.

Rather than sending ICMP Echo requests, tcping performs a **TCP SYN / SYN-ACK handshake** on a target port and measures the **round-trip time (RTT)** for each exchange — without completing a full connection. This makes it particularly useful in environments where ICMP is filtered or blocked.

It also supports setting the **TOS (Type of Service)** field directly in the IP header, enabling end-to-end QoS and DSCP marking verification.

> ⚠️ **Requires root privileges** — raw sockets with `IP_HDRINCL` are used to craft and inject packets at the network layer.

---

## ✨ Features

- 🤝 TCP SYN / SYN-ACK handshake-based RTT measurement (no full connection)
- 📊 Per-probe output: sequence number + RTT in milliseconds
- ⏱️ Timeout detection per probe with `timeout...` feedback
- 📉 Final statistics on `^C` or end of count: transmitted, received, loss %, min/avg/max RTT
- 🏷️ **TOS/DSCP field support** (`-Q`) for QoS path testing
- 🔁 **3-thread architecture**: dedicated threads for sending, sniffing, and timeout management
- 🌐 Auto-detection of local source IP (`en0`, `eth0`, `enp0s3`)
- 🔧 Fully configurable: port, count, interval, timeout, source address

---

## 🚀 Usage

```bash
./tcping [-d destination addr] [-s source addr]
         [-t timeout] [-c count] [-p port] [-Q tos]
         [-i interval]
```

### Options

| Flag | Description | Default |
|------|-------------|---------|
| `-d` | Destination IP address (**required**) | — |
| `-p` | Target TCP port | `80` |
| `-s` | Source IP address (overrides auto-detect) | Auto (`en0`/`eth0`/`enp0s3`) |
| `-t` | Timeout per probe in seconds | `3` |
| `-c` | Number of probes to send (`-1` = unlimited) | Unlimited |
| `-Q` | IP TOS byte value for QoS/DSCP marking | `0` |
| `-i` | Interval between probes in seconds (float) | `1` |
| `-h` | Display usage | — |

---

## 💡 Examples

### Basic TCP ping on port 443

```bash
sudo ./tcping -d 8.8.8.8 -p 443
```

**Output:**
```
from (8.8.8.8): tcp_seq=1  time=4.25 ms
from (8.8.8.8): tcp_seq=2  time=4.62 ms
from (8.8.8.8): tcp_seq=3  time=4.68 ms
...
from (8.8.8.8): tcp_seq=20 time=14.61 ms
^C

--- 8.8.8.8 tcping statistics ---
20 packets transmitted, 20 received, 0.00% packet loss
rtt min/avg/max = 3.64/5.62/14.61 ms
```

### Send exactly 5 probes, 2-second interval

```bash
sudo ./tcping -d 192.168.1.1 -p 80 -c 5 -i 2
```

### QoS testing — mark packets with DSCP EF (Expedited Forwarding)

```bash
sudo ./tcping -d 10.0.0.1 -p 443 -Q 184
```

> `184` = `0xB8` = DSCP EF (46) — used to verify QoS priority handling end-to-end.

### Probe a DNS server on port 53 with custom source and timeout

```bash
sudo ./tcping -d 1.1.1.1 -p 53 -s 192.168.0.10 -t 5
```

---

## 🛠️ Build

### Prerequisites

- GCC or Clang
- Linux (or macOS with raw socket support)
- `make`
- `pthread` library
- Root access or `CAP_NET_RAW` capability

### Compile

```bash
make
```

### Clean

```bash
make clean
```

### Run

```bash
sudo ./tcping -d  -p 
```

---

## 🗂️ Project Structure

```
tcping/
├── includes/
│   └── tcping.h      # Structs, typedefs, color macros, function prototypes
├── srcs/
│   ├── tcping.c      # Entry point: arg parsing, main loop, socket init (IP_HDRINCL)
│   ├── options.c     # Local IP detection (getifaddrs), checksum (csum), RTT calc, stats update
│   ├── socket.c      # Packet init: IP header, TCP SYN header, pseudo-header, MSS option
│   ├── send.c        # TCP SYN packet injection via raw socket (sendto)
│   ├── sniffer.c     # Raw socket sniffing, SYN-ACK / RST-ACK filtering (process_packet)
│   ├── handle.c      # SIGINT handler: prints final statistics and exits
│   └── thread.c      # 3-thread orchestration: threadSendSyn, threadSniffer, threadSleep
└── Makefile
```

---

## 🧠 How It Works

For each probe, tcping spawns **3 concurrent POSIX threads**:

```
┌──────────────────────────────────────────────────────┐
│                      tcping                          │
│                                                      │
│  threadSendSyn       threadSniffer     threadSleep   │
│  (send.c)            (sniffer.c)       (timeout)     │
│      │                    ▲                │         │
│  gettimeofday()           │            sleep(t)      │
│  sendSyn() ───────────────┼──────────►     │         │
│  [SYN, TOS=Q]             │            if timeout:   │
│                     SYN-ACK received   cancel        │
│                     gettimeofday()     threadSniffer │
│                     stats.rcv++                      │
│                     cancel threadSleep               │
└──────────────────────────────────────────────────────┘
         RTT = time(SYN-ACK received) - time(SYN sent)
```

## 📐 TOS / DSCP Reference

The `-Q` flag sets the **IP TOS byte** directly in the IP header. Common DSCP values:

| DSCP Name | DSCP Value | TOS (decimal) | Use case |
|-----------|------------|---------------|----------|
| Default BE | 0         | 0             | Best-effort |
| AF11       | 10        | 40            | Low-priority data |
| AF41       | 34        | 136           | Video conferencing |
| EF         | 46        | 184           | Voice / real-time |
| CS6        | 48        | 192           | Network control |

---

## ⚠️ Disclaimer

This tool uses **raw sockets** (`SOCK_RAW` + `IP_HDRINCL`) and sends **crafted TCP packets**. Usage must be restricted to:

- Networks and systems **you own or are authorized to test**
- Controlled lab or production environments with proper authorization

Unauthorized packet injection may violate local laws and network policies.

---

## 👤 Author

**Tybbow** (tiskow@student.42.fr) — v1.0  
🔗 [github.com/Tybbow/tcping](https://github.com/Tybbow/tcping)

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).
