#include <LEDA/core/string.h>

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <resolv.h>
#include <netdb.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <arpa/inet.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

using std::ostream;
using std::cerr;
using std::endl;

LEDA_BEGIN_NAMESPACE

class tls_socket_impl 
{
  SSL_CTX* ctx;
  SSL* ssl;
  int fd;

  string host_name;
  int port_num;

  string error_msg;


bool create_ssl_context()
{
  SSL_library_init();
  OpenSSL_add_all_algorithms();  
  SSL_load_error_strings();   

/*
OPENSSL_config(NULL);
*/

  //const SSL_METHOD* method = TLS_method();
  const SSL_METHOD* method = TLS_client_method();

  ctx = SSL_CTX_new(method);   

/*
  SSL_CTX_set_verify(ctx,mode,callback);
*/

  SSL_CTX_set_options(ctx,SSL_OP_ALL);
  SSL_CTX_set_options(ctx,SSL_OP_NO_TLSv1);
  SSL_CTX_set_options(ctx,SSL_OP_NO_SSLv2);

/*
  SSL_CTX_set_options(ctx,SSL_OP_NO_COMPRESSION);
  SSL_CTX_set_options(ctx,SSL_OP_NO_SSLv3);
  SSL_CTX_set_options(ctx,SSL_OP_NO_TLSv1);
  SSL_CTX_set_options(ctx,SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION);
*/


  if ( ctx == NULL )
  { error_msg = string("SSL Context: ") + ERR_error_string(ERR_get_error(),0); 
    return false;
  }
  return true;
}


void configure_context(SSL_CTX *ctx)
{
    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}


void release_ssl_context() {
   SSL_CTX_free(ctx); 
}


public:

void show_certificates(ostream& out)
{
  X509* cert = SSL_get_peer_certificate(ssl); 

  if ( cert == NULL )
  { out << "No certificates configured." << endl;
    return;
   }

  out << "Server certificates" << endl;

  char* line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
  out << "Subject: " << line << endl;;
  delete[] line;

  line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
  out << "Issuer: " << line << endl;;
  delete[] line;

  X509_free(cert);

  out << "Server Encryption: " <<  SSL_get_cipher(ssl) << endl;
}


tls_socket_impl(string host="", int port=0) {
  ssl = 0;
  fd = -1;
  host_name = host;
  port_num = port;
  create_ssl_context();
}


~tls_socket_impl() {
   release_ssl_context();
   disconnect();
 }

void set_host(string host) { host_name = host; }
void set_port(int port) { port_num = port; }


bool connect(int timeout)
{
  hostent* host = gethostbyname(host_name);

  if (host == NULL )
  { error_msg = strerror(errno);
    return false;
  }

  //cerr << "CONNECT: " << host_name << " " << port_num << endl;

  fd = ::socket(AF_INET, SOCK_STREAM, 0);

  if (fd == -1) {
    error_msg = strerror(errno);
    return false;
  }

  int enable = 1;
  setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&enable,sizeof(int));
//setsockopt(fd,SOL_SOCKET,SO_REUSEPORT,&enable,sizeof(int));

  if (timeout != 0) 
  { // set fd to non-blocking 
    long arg = fcntl(fd, F_GETFL, NULL); 
    arg |= O_NONBLOCK; 
    fcntl(fd, F_SETFL, arg); 
   }
 
// connect to server

  sockaddr_in addr;
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port_num);
  addr.sin_addr.s_addr = *(long*)(host->h_addr);

  if (::connect(fd, (sockaddr*)&addr, sizeof(addr)) < 0) 
  { 
    if (timeout == 0)
    { error_msg = strerror(errno);
      return false;
    }

    // connect with timeout (seconds)

    timeval poll_t; 
    poll_t.tv_sec = timeout; 
    poll_t.tv_usec = 0; 
    fd_set wrset; 
    FD_ZERO(&wrset); 
    FD_SET(fd, &wrset); 

    if (select(fd+1, NULL, &wrset, NULL, &poll_t)) 
     { socklen_t len = sizeof(int); 
       int valopt = 0; 
       getsockopt(fd, SOL_SOCKET, SO_ERROR, &valopt, &len); 
       if (valopt) 
       { error_msg = string("socket::connect failed (err = %d)",valopt); 
         return false;
        } 
     } 
    else 
     { error_msg = "connect timeout"; 
       return false;
     } 
  } 

  if (timeout != 0)
  { // set back to blocking mode 
    long arg = fcntl(fd, F_GETFL, NULL); 
    arg &= (~O_NONBLOCK); 
    fcntl(fd, F_SETFL, arg); 
   }


  // create new ssl structure and connect

  ssl = SSL_new(ctx);  

  SSL_set_fd(ssl,fd); 

  if (SSL_connect(ssl) == -1) 
  { error_msg = string("SSL connect: %s %d\n", ~host_name, port_num);
    error_msg += ERR_error_string(ERR_get_error(),0); 
    return false;
    }

  return true;
}

bool connected() { return ssl != 0; }

int  get_fd() const { return fd; }

string get_error() const { return error_msg; }

string client_ip() const { return ""; }


void disconnect()
{ // release ssl connection  and close socket
  if (ssl) { SSL_free(ssl); ssl = 0; }
  if (fd) { close(fd);  fd = 0; }
}

void detach()
{ // close socket endpoint
  if (fd) { close(fd);  fd = 0; }
}


size_t receive(char* buf, size_t buf_sz) {
  return SSL_read(ssl,buf,buf_sz); 
}


void send(char* buf, size_t sz) {
  SSL_write(ssl,buf,sz); 
}

};

LEDA_END_NAMESPACE
