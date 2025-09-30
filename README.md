# ft_irc - Internet Relay Chat

## 🎯 Project Overview

**ft_irc** is a project that involves creating your own IRC (Internet Relay Chat) server from scratch. You'll use an actual IRC client to connect to your server and test its functionality, gaining deep understanding of internet communication protocols.

> **Summary:** This project is about creating your own IRC server. You will use an actual IRC client to connect to your server and test it. The Internet is governed by solid standard protocols that allow connected computers to interact with each other. It's always beneficial to understand these protocols.

## 🌐 What is IRC?

**Internet Relay Chat** is a text-based communication protocol on the Internet. It offers real-time messaging that can be either public or private. Users can exchange direct messages and join group channels.

IRC clients connect to IRC servers to join channels, forming communication networks.

## ⚙️ Technical Specifications

### Program Requirements
| Component | Specification |
|-----------|---------------|
| **Program Name** | `ircserv` |
| **Language** | C++ 98 Standard |
| **Compilation** | `c++` with `-Wall -Wextra -Werror -std=c++98` |
| **Execution** | `./ircserv <port> <password>` |
| **I/O Model** | Non-blocking, single `poll()` (or equivalent) |

### Key Constraints
- ✅ **Must handle multiple clients simultaneously**
- ✅ **All I/O operations must be non-blocking**
- ✅ **Only one `poll()` call for all operations**
- ❌ **Forking is prohibited**
- ❌ **No external libraries or Boost**
- ❌ **No server-to-server communication**
- ❌ **No IRC client development**

## 🔧 Core Features

### Authentication & Basic Commands
- **Authentication** with server password
- **NICK** - Set user nickname
- **USER** - Set username
- **JOIN** - Join channels
- **PRIVMSG** - Send private messages
- **Channel messaging** with message forwarding to all channel members

### User Management
- **Operator and regular user roles**
- **Operator-specific commands**:
  - **KICK** - Eject client from channel
  - **INVITE** - Invite client to channel
  - **TOPIC** - Change/view channel topic
  - **MODE** - Channel mode management

### Channel Modes
- **i** - Set/remove Invite-only channel
- **t** - Restrict TOPIC command to operators
- **k** - Set/remove channel password
- **o** - Give/take operator privileges
- **l** - Set/remove user limit

## 🛠️ Technical Implementation

### Network Programming
- **TCP/IP communication** (IPv4 or IPv6)
- **Non-blocking file descriptors**
- **Single `poll()` call** for all I/O operations
- **Alternative I/O multiplexing** allowed: `select()`, `kqueue()`, `epoll()`
  We used epoll

### Data Handling
- **Packet aggregation** for command reconstruction
- **Partial data reception** handling
- **Error processing** for all edge cases
- **Resource-efficient** operation

Client used: `HexChat`
I/O operations: `epoll`
