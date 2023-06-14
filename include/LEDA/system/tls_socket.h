/*******************************************************************************
+
+  LEDA 7.0  
+
+
+  socket.h
+
+
+  Copyright (c) 1995-2023
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_TLS_SOCKET_H
#define LEDA_TLS_SOCKET_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 669950
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/system/socket.h>

LEDA_BEGIN_NAMESPACE

//class __exportC tls_socket : public socket 

class tls_socket : public socket 
{
  void* sock_ptr;

public:

 tls_socket(string host="", int port=0);
~tls_socket();

void set_host(string host);
void set_port(int port);

bool connected();

// server
bool listen();
bool accept();

//client
bool connect();
bool connect(int sec);

void disconnect();
void detach();

void send(char* buffer, size_t size);
void send(string s);

size_t receive(char* buf, size_t sz);
size_t receive(char* buf, size_t sz, int sec);

bool poll(int msec);

string get_error() const;

string client_ip() const;

int sock_fd() const;

void   show_certificates(ostream& out);

};

LEDA_END_NAMESPACE



#if defined(__win32__) || defined(__win64__)
#include <LEDA/system/tls_win_socket.h>
#else
#include <LEDA/system/tls_unix_socket.h>
#endif


LEDA_BEGIN_NAMESPACE

#define SOCK ((tls_socket_impl*)sock_ptr)

tls_socket::tls_socket(string host, int port) {
  sock_ptr = new tls_socket_impl(host,port);
}

tls_socket::~tls_socket() { 
  delete SOCK;
}

void tls_socket::set_host(string host) { 
  SOCK->set_host(host);
}

void tls_socket::set_port(int port) {
  SOCK->set_port(port);
}

// server
bool tls_socket::listen() { return false; }
bool tls_socket::accept() { return false; }


//client


bool tls_socket::connect() { 
  return SOCK->connect(0);
}

bool tls_socket::connect(int sec) { 
  return SOCK->connect(sec);
}

void tls_socket::disconnect() { 
  SOCK->disconnect(); 
}

void tls_socket::detach() { 
  SOCK->detach(); 
}

bool tls_socket::connected() { 
  return SOCK->connected(); 
}


void tls_socket::send(char* buffer, size_t size) { 
  SOCK->send(buffer,size);
}

void tls_socket::send(string s) { 
  SOCK->send((char*)s.cstring(),s.length());
}

size_t tls_socket::receive(char* buf, size_t sz) { 
  return SOCK->receive(buf,sz); 
}

size_t tls_socket::receive(char* buf, size_t sz, int sec) { 
 if (poll(1000*sec)) 
   return receive(buf,sz); 
 else
   return 0;
}

bool tls_socket::poll(int msec)
{ 
  int fd = SOCK->get_fd(); 

  timeval polltime;
  polltime.tv_sec  = msec / 1000;
  polltime.tv_usec = 1000 * (msec % 1000);

  fd_set rdset, wrset, xset;
  FD_ZERO(&rdset);
  FD_SET(fd,&rdset);
  FD_ZERO(&wrset);
  FD_ZERO(&xset);
  FD_SET(fd,&xset);
  int n = select(fd+1,&rdset,&wrset,&xset,&polltime);

  return n > 0;
}


string tls_socket::get_error() const { return SOCK->get_error(); }

string tls_socket::client_ip() const { return SOCK->client_ip(); }

int    tls_socket::sock_fd() const { return SOCK->get_fd(); }

void   tls_socket::show_certificates(ostream& out) {}


LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 669950
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif

