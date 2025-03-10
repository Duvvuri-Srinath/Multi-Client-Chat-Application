# Multi-Client Chat Application  
**C++, Linux Sockets, Multithreading, Epoll**  
---

## 🚀 Overview  
This is a **high-performance multi-client chat application** built in **C++** using **Linux Sockets, Multithreading, and Epoll** for efficient real-time messaging. The server can handle multiple clients concurrently, and clients can send messages to each other through the server.

---

## 📌 Features  
✔ **Multi-Client Support** – Uses **epoll** for efficient handling of multiple clients.  
✔ **Concurrent Server** – Handles multiple connections using **multithreading**.  
✔ **Custom Messaging Protocol** – Implements message framing and error handling.  
✔ **Secure Communication** – Supports **basic encryption** for data exchange.  
✔ **Non-Blocking I/O** – Uses **epoll()** to efficiently manage client connections.  
✔ **User Authentication** – Simple login system for secure access.  
✔ **Message Logging** – Stores chat history for later retrieval.  

---

## 🛠️ Technology Stack  
- **C++** – Core programming language.  
- **Linux Sockets** – For TCP/IP communication.  
- **Multithreading (pthread)** – For concurrent request handling.  
- **Epoll** – For non-blocking, event-driven network handling.  
- **OpenSSL (optional)** – For encrypted communication.  

---

## ⚙️ Installation & Setup  

### Clone the Repository  
```bash
git clone https://github.com/yourusername/MultiClientChat.git
cd MultiClientChat
```

### Compile the Server & Client
``` bash
g++ -o server server.cpp -lpthread
g++ -o client client.cpp
```

### Run the Server
``` bash
./server
```

### Run Multiple Clients
``` bash
./client
```

### 🖥️ Usage

### Server Commands
- exit – Shuts down the server.
- list – Displays all connected clients.

### Client Commands
- /msg <username> <message> – Send a private message.
- /all <message> – Broadcast a message to all users.
- /quit – Disconnect from the chat.

### 📜 License
This project is licensed under the MIT License – Feel free to use and modify it!

### Author
Developed by Srinath Duvvuri.
