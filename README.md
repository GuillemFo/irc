*****AI used to explain this project*****
# ft_irc - Internet Relay Chat

## 🎯 Project Overview

**ft_irc** is a networking project that consists of building a fully functional **IRC (Internet Relay Chat) server** from scratch using **C++98**.

The goal is to implement the core features of the IRC protocol so that real IRC clients can connect to the server, authenticate, join channels, and exchange messages in real time.

This project provides practical experience with:

* **Network programming**
* **TCP/IP communication**
* **Non-blocking I/O**
* **Event-driven server architecture**
* **Protocol implementation**

By recreating the behavior of an IRC server, the project helps develop a deeper understanding of how internet communication protocols work and how servers handle multiple simultaneous clients.

---

## 🌐 What is IRC?

**Internet Relay Chat (IRC)** is a text-based communication protocol used for real-time messaging over the Internet.

It allows users to communicate through:

* **Private messages** between individual users
* **Public discussions** inside channels (chat rooms)

Users connect to an **IRC server** using an IRC client.
Once connected, they can join channels, send messages, and interact with other users connected to the same server.

IRC was one of the earliest real-time communication systems on the Internet and remains an important example of how messaging protocols and client-server architectures operate.

---

## ⚙️ Technical Specifications

### Program Requirements

| Component        | Specification                                 |
| ---------------- | --------------------------------------------- |
| **Program Name** | `ircserv`                                     |
| **Language**     | C++ 98 Standard                               |
| **Compilation**  | `c++` with `-Wall -Wextra -Werror -std=c++98` |
| **Execution**    | `./ircserv <port> <password>`                 |
| **I/O Model**    | Non-blocking, single `poll()` (or equivalent) |

### Key Constraints

* ✅ **Handle multiple clients simultaneously**
* ✅ **All I/O operations must be non-blocking**
* ✅ **Use a single `poll()` call for event handling**
* ❌ **Forking is prohibited**
* ❌ **No external libraries or Boost**
* ❌ **No server-to-server communication**
* ❌ **No IRC client development**

---

## 🔧 Core Features

### Authentication & Basic Commands

* **Server authentication** using a password
* **NICK** — Set or change user nickname
* **USER** — Register user information
* **JOIN** — Join or create channels
* **PRIVMSG** — Send private messages
* **Channel messaging** with broadcast to all members

### User Management

The server distinguishes between **regular users** and **channel operators**.

Operator privileges allow the use of special moderation commands:

* **KICK** — Remove a user from a channel
* **INVITE** — Invite a user to an invite-only channel
* **TOPIC** — Set or view the channel topic
* **MODE** — Manage channel modes and permissions

---

## 🔒 Channel Modes

Channel operators can modify channel behavior using the **MODE** command.

| Mode  | Description                            |
| ----- | -------------------------------------- |
| **i** | Invite-only channel                    |
| **t** | Only operators can change the topic    |
| **k** | Channel requires a password            |
| **o** | Grant or remove operator privileges    |
| **l** | Limit the number of users in a channel |

---

## 🛠️ Technical Implementation

### Network Programming

* **TCP/IP communication** (IPv4 / IPv6)
* **Non-blocking sockets**
* **Event-driven architecture**
* **I/O multiplexing**

Although the project allows different multiplexing mechanisms (`poll`, `select`, `kqueue`, `epoll`), this implementation uses:

* **epoll** for efficient event handling.

### Data Handling

The server includes logic to properly handle real-world network behavior:

* **Packet aggregation** for reconstructing IRC commands
* **Partial message handling**
* **Robust error handling**
* **Efficient resource management**

---

## 💻 Client Used for Testing

The server was tested using the IRC client:

* **HexChat**

Additional documentation and technical notes can be found in the **`zInfo`** folder.
