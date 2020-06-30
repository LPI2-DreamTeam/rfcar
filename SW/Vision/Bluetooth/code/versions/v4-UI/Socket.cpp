/**
 * @file Socket.cpp
 * @author Jose Pires
 * @date 03-06-2019
 *
 * @brief Implementation for a network programming socket
 *
 * It creates a socket and retrieves a handle to it.
 * - It must be specified the domain, type and protocol to be useful.
 *   Thus, the need for specialization via subclasses.
 */
#include "Socket.h"
#include <sys/time.h> /* Required for struct timeval */
#include <sys/types.h>         /* Socket calls compatibility */ 
#include <sys/socket.h> /* socket calls */
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <string.h> /* strerror */

#define RECV_BUF_LIM ((size_t)4098)

using std::runtime_error;

/* Used to create abstract class; it must be declared like this, but must also
 * be implemented, so subclasses can destroy the base class */
Socket::~Socket(){ 
    Close();
}

Socket::Socket(int domain, int type, int protocol) : 
    rcv_len(1024), conn_timeout(0), connected(false)
{
    /* 
DESCRIPTION         top

       socket() creates an endpoint for communication and returns a file
       descriptor that refers to that endpoint.  The file descriptor
       returned by a successful call will be the lowest-numbered file
       descriptor not currently open for the process.

       The domain argument specifies a communication domain; this selects
       the protocol family which will be used for communication.  These
       families are defined in <sys/socket.h>.  The formats currently
       understood by the Linux kernel include:

       Name         Purpose                                    Man page
       AF_UNIX      Local communication                        unix(7)
       AF_LOCAL     Synonym for AF_UNIX
       AF_INET      IPv4 Internet protocols                    ip(7)
       AF_AX25      Amateur radio AX.25 protocol               ax25(4)
       AF_IPX       IPX - Novell protocols
       AF_APPLETALK AppleTalk                                  ddp(7)
       AF_X25       ITU-T X.25 / ISO-8208 protocol             x25(7)
       AF_INET6     IPv6 Internet protocols                    ipv6(7)
       AF_DECnet    DECet protocol sockets
       AF_KEY       Key management protocol, originally
                    developed for usage with IPsec
       AF_NETLINK   Kernel user interface device               netlink(7)
       AF_PACKET    Low-level packet interface                 packet(7)
       AF_RDS       Reliable Datagram Sockets (RDS) protocol   rds(7)
                                                               rds-rdma(7)
       AF_PPPOX     Generic PPP transport layer, for setting
                    up up L2 tunnels (L2TP and PPPoE)
       AF_LLC       Logical link control (IEEE 802.2 LLC)
                    protocol
       AF_IB        InfiniBand native addressing
       AF_MPLS      Multiprotocol Label Switching
       AF_CAN       Controller Area Network automotive bus
                    protocol
       AF_TIPC      TIPC, "cluster domain sockets" protocol
       AF_BLUETOOTH Bluetooth low-level socket protocol
       AF_ALG       Interface to kernel crypto API
       AF_VSOCK     VSOCK (originally "VMWare VSockets")       vsock(7)
                    protocol for hypervisor-guest
                    communication
       AF_KCM       KCM (kernel connection multiplexor)
                    interface
       AF_XDP       XDP (express data path) interface

       Further details of the above address families, as well as information
       on several other address families, can be found in
       address_families(7).

       The socket has the indicated type, which specifies the communication
       semantics.  Currently defined types are:

       SOCK_STREAM     Provides sequenced, reliable, two-way, connection-
                       based byte streams.  An out-of-band data transmission
                       mechanism may be supported.

       SOCK_DGRAM      Supports datagrams (connectionless, unreliable
                       messages of a fixed maximum length).

       SOCK_SEQPACKET  Provides a sequenced, reliable, two-way connection-
                       based data transmission path for datagrams of fixed
                       maximum length; a consumer is required to read an
                       entire packet with each input system call.

       SOCK_RAW        Provides raw network protocol access.

       SOCK_RDM        Provides a reliable datagram layer that does not
                       guarantee ordering.

       SOCK_PACKET     Obsolete and should not be used in new programs; see
                       packet(7).

       Some socket types may not be implemented by all protocol families.

       Since Linux 2.6.27, the type argument serves a second purpose: in
       addition to specifying a socket type, it may include the bitwise OR
       of any of the following values, to modify the behavior of socket():

       SOCK_NONBLOCK   Set the O_NONBLOCK file status flag on the open file
                       description (see open(2)) referred to by the new file
                       descriptor.  Using this flag saves extra calls to
                       fcntl(2) to achieve the same result.

       SOCK_CLOEXEC    Set the close-on-exec (FD_CLOEXEC) flag on the new
                       file descriptor.  See the description of the
                       O_CLOEXEC flag in open(2) for reasons why this may be
                       useful.

       The protocol specifies a particular protocol to be used with the
       socket.  Normally only a single protocol exists to support a
       particular socket type within a given protocol family, in which case
       protocol can be specified as 0.  However, it is possible that many
       protocols may exist, in which case a particular protocol must be
       specified in this manner.  The protocol number to use is specific to
       the “communication domain” in which communication is to take place;
       see protocols(5).  See getprotoent(3) on how to map protocol name
       strings to protocol numbers.

       Sockets of type SOCK_STREAM are full-duplex byte streams.  They do
       not preserve record boundaries.  A stream socket must be in a
       connected state before any data may be sent or received on it.  A
       connection to another socket is created with a connect(2) call.  Once
       connected, data may be transferred using read(2) and write(2) calls
       or some variant of the send(2) and recv(2) calls.  When a session has
       been completed a close(2) may be performed.  Out-of-band data may
       also be transmitted as described in send(2) and received as described
       in recv(2).

       The communications protocols which implement a SOCK_STREAM ensure
       that data is not lost or duplicated.  If a piece of data for which
       the peer protocol has buffer space cannot be successfully transmitted
       within a reasonable length of time, then the connection is considered
       to be dead.  When SO_KEEPALIVE is enabled on the socket the protocol
       checks in a protocol-specific manner if the other end is still alive.
       A SIGPIPE signal is raised if a process sends or receives on a broken
       stream; this causes naive processes, which do not handle the signal,
       to exit.  SOCK_SEQPACKET sockets employ the same system calls as
       SOCK_STREAM sockets.  The only difference is that read(2) calls will
       return only the amount of data requested, and any data remaining in
       the arriving packet will be discarded.  Also all message boundaries
       in incoming datagrams are preserved.

       SOCK_DGRAM and SOCK_RAW sockets allow sending of datagrams to
       correspondents named in sendto(2) calls.  Datagrams are generally
       received with recvfrom(2), which returns the next datagram along with
       the address of its sender.

       SOCK_PACKET is an obsolete socket type to receive raw packets
       directly from the device driver.  Use packet(7) instead.

       An fcntl(2) F_SETOWN operation can be used to specify a process or
       process group to receive a SIGURG signal when the out-of-band data
       arrives or SIGPIPE signal when a SOCK_STREAM connection breaks
       unexpectedly.  This operation may also be used to set the process or
       process group that receives the I/O and asynchronous notification of
       I/O events via SIGIO.  Using F_SETOWN is equivalent to an ioctl(2)
       call with the FIOSETOWN or SIOCSPGRP argument.

       When the network signals an error condition to the protocol module
       (e.g., using an ICMP message for IP) the pending error flag is set
       for the socket.  The next operation on this socket will return the
       error code of the pending error.  For some protocols it is possible
       to enable a per-socket error queue to retrieve detailed information
       about the error; see IP_RECVERR in ip(7).

       The operation of sockets is controlled by socket level options.
       These options are defined in <sys/socket.h>.  The functions
       setsockopt(2) and getsockopt(2) are used to set and get options.

RETURN VALUE         top

       On success, a file descriptor for the new socket is returned.  On
       error, -1 is returned, and errno is set appropriately.

     */ 
    int status = socket(domain, type, protocol); 
    if(status < 0)
    {
        string s = "[Socket]: Failed to create - ";
        s += string( strerror(errno) );
        throw runtime_error(s);
    }
    /* Store handle to socket */
    this->handle = status;
    /* Initialize remote socket handle */
    this->rhandle = -1;
    /* Set the socket as non blocking */
    this->setTimeout(this->handle, 0);
    /* Clear file descriptor sets of read and write */
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    /* Track socket for reading and writing */
    addToTracking(this->handle);
}

void Socket::Close()
{
    /* Close file descriptor */
    close(this->handle);
}

int Socket::Send(const string & msg)
{
    if(!this->connected)
        throw runtime_error("[Socket]: Socket must be connect to send");

    /* TODO: get the appropriate flag */
    int flags = 0; 

/* 
DESCRIPTION         top

       The system calls send(), sendto(), and sendmsg() are used to transmit
       a message to another socket.

       The send() call may be used only when the socket is in a connected
       state (so that the intended recipient is known).  The only difference
       between send() and write(2) is the presence of flags.  With a zero
       flags argument, send() is equivalent to write(2).  Also, the
       following call

           send(sockfd, buf, len, flags);

       is equivalent to

           sendto(sockfd, buf, len, flags, NULL, 0);

       The argument sockfd is the file descriptor of the sending socket.

       If sendto() is used on a connection-mode (SOCK_STREAM,
       SOCK_SEQPACKET) socket, the arguments dest_addr and addrlen are
       ignored (and the error EISCONN may be returned when they are not NULL
       and 0), and the error ENOTCONN is returned when the socket was not
       actually connected.  Otherwise, the address of the target is given by
       dest_addr with addrlen specifying its size.  For sendmsg(), the
       address of the target is given by msg.msg_name, with msg.msg_namelen
       specifying its size.

       For send() and sendto(), the message is found in buf and has length
       len.  For sendmsg(), the message is pointed to by the elements of the
       array msg.msg_iov.  The sendmsg() call also allows sending ancillary
       data (also known as control information).

       If the message is too long to pass atomically through the underlying
       protocol, the error EMSGSIZE is returned, and the message is not
       transmitted.

       No indication of failure to deliver is implicit in a send().  Locally
       detected errors are indicated by a return value of -1.

       When the message does not fit into the send buffer of the socket,
       send() normally blocks, unless the socket has been placed in
       nonblocking I/O mode.  In nonblocking mode it would fail with the
       error EAGAIN or EWOULDBLOCK in this case.  The select(2) call may be
       used to determine when it is possible to send more data.

   The flags argument
       The flags argument is the bitwise OR of zero or more of the following
       flags.

       MSG_CONFIRM (since Linux 2.3.15)
              Tell the link layer that forward progress happened: you got a
              successful reply from the other side.  If the link layer
              doesn't get this it will regularly reprobe the neighbor (e.g.,
              via a unicast ARP).  Valid only on SOCK_DGRAM and SOCK_RAW
              sockets and currently implemented only for IPv4 and IPv6.  See
              arp(7) for details.

       MSG_DONTROUTE
              Don't use a gateway to send out the packet, send to hosts only
              on directly connected networks.  This is usually used only by
              diagnostic or routing programs.  This is defined only for
              protocol families that route; packet sockets don't.

       MSG_DONTWAIT (since Linux 2.2)
              Enables nonblocking operation; if the operation would block,
              EAGAIN or EWOULDBLOCK is returned.  This provides similar
              behavior to setting the O_NONBLOCK flag (via the fcntl(2)
              F_SETFL operation), but differs in that MSG_DONTWAIT is a per-
              call option, whereas O_NONBLOCK is a setting on the open file
              description (see open(2)), which will affect all threads in
              the calling process and as well as other processes that hold
              file descriptors referring to the same open file description.

       MSG_EOR (since Linux 2.2)
              Terminates a record (when this notion is supported, as for
              sockets of type SOCK_SEQPACKET).

       MSG_MORE (since Linux 2.4.4)
              The caller has more data to send.  This flag is used with TCP
              sockets to obtain the same effect as the TCP_CORK socket
              option (see tcp(7)), with the difference that this flag can be
              set on a per-call basis.

              Since Linux 2.6, this flag is also supported for UDP sockets,
              and informs the kernel to package all of the data sent in
              calls with this flag set into a single datagram which is
              transmitted only when a call is performed that does not
              specify this flag.  (See also the UDP_CORK socket option
              described in udp(7).)

       MSG_NOSIGNAL (since Linux 2.2)
              Don't generate a SIGPIPE signal if the peer on a stream-
              oriented socket has closed the connection.  The EPIPE error is
              still returned.  This provides similar behavior to using
              sigaction(2) to ignore SIGPIPE, but, whereas MSG_NOSIGNAL is a
              per-call feature, ignoring SIGPIPE sets a process attribute
              that affects all threads in the process.

       MSG_OOB
              Sends out-of-band data on sockets that support this notion
              (e.g., of type SOCK_STREAM); the underlying protocol must also
              support out-of-band data.

   sendmsg()
       The definition of the msghdr structure employed by sendmsg() is as
       follows:

           struct msghdr {
               void         *msg_name;       // Optional address 
               socklen_t     msg_namelen;    // Size of address 
               struct iovec *msg_iov;        // Scatter/gather array 
               size_t        msg_iovlen;     // # elements in msg_iov 
               void         *msg_control;    // Ancillary data, see below 
               size_t        msg_controllen; // Ancillary data buffer len 
               int           msg_flags;      // Flags (unused) 
           };

       The msg_name field is used on an unconnected socket to specify the
       target address for a datagram.  It points to a buffer containing the
       address; the msg_namelen field should be set to the size of the
       address.  For a connected socket, these fields should be specified as
       NULL and 0, respectively.

       The msg_iov and msg_iovlen fields specify scatter-gather locations,
       as for writev(2).

       You may send control information using the msg_control and msg_con‐
       trollen members.  The maximum control buffer length the kernel can
       process is limited per socket by the value in /proc/sys/net/core/opt‐
       mem_max; see socket(7).

       The msg_flags field is ignored.

RETURN VALUE         top

       On success, these calls return the number of bytes sent.  On error,
       -1 is returned, and errno is set appropriately.
 */
    int status = send(this->handle, (const void *)msg.c_str(), 
                      msg.length(), flags);
    if(status < 0 && ( (errno != EAGAIN) || (errno != EWOULDBLOCK) ) ) 
    {
        string s = "[Socket]: Failed to send - ";
        s += string( strerror(errno) );
        throw runtime_error(s);
    }
    return status; // return nr. of bytes send or -1 (errno should be EAGAIN)
}

int Socket::Recv(string &s )
{
    if(!this->connected)
        throw runtime_error("[Socket]: Socket must be connect to receive");
    /* TODO: get the appropriate flag */
    int flags = 0; 
    char buf[1024] = {0};
/* 
DESCRIPTION         top

       The recv(), recvfrom(), and recvmsg() calls are used to receive
       messages from a socket.  They may be used to receive data on both
       connectionless and connection-oriented sockets.  This page first
       describes common features of all three system calls, and then
       describes the differences between the calls.

       The only difference between recv() and read(2) is the presence of
       flags.  With a zero flags argument, recv() is generally equivalent to
       read(2) (but see NOTES).  Also, the following call

           recv(sockfd, buf, len, flags);

       is equivalent to

           recvfrom(sockfd, buf, len, flags, NULL, NULL);

       All three calls return the length of the message on successful
       completion.  If a message is too long to fit in the supplied buffer,
       excess bytes may be discarded depending on the type of socket the
       message is received from.

       If no messages are available at the socket, the receive calls wait
       for a message to arrive, unless the socket is nonblocking (see
       fcntl(2)), in which case the value -1 is returned and the external
       variable errno is set to EAGAIN or EWOULDBLOCK.  The receive calls
       normally return any data available, up to the requested amount,
       rather than waiting for receipt of the full amount requested.

       An application can use select(2), poll(2), or epoll(7) to determine
       when more data arrives on a socket.

   The flags argument
       The flags argument is formed by ORing one or more of the following
       values:

       MSG_CMSG_CLOEXEC (recvmsg() only; since Linux 2.6.23)
              Set the close-on-exec flag for the file descriptor received
              via a UNIX domain file descriptor using the SCM_RIGHTS
              operation (described in unix(7)).  This flag is useful for the
              same reasons as the O_CLOEXEC flag of open(2).

       MSG_DONTWAIT (since Linux 2.2)
              Enables nonblocking operation; if the operation would block,
              the call fails with the error EAGAIN or EWOULDBLOCK.  This
              provides similar behavior to setting the O_NONBLOCK flag (via
              the fcntl(2) F_SETFL operation), but differs in that
              MSG_DONTWAIT is a per-call option, whereas O_NONBLOCK is a
              setting on the open file description (see open(2)), which will
              affect all threads in the calling process and as well as other
              processes that hold file descriptors referring to the same
              open file description.

       MSG_ERRQUEUE (since Linux 2.2)
              This flag specifies that queued errors should be received from
              the socket error queue.  The error is passed in an ancillary
              message with a type dependent on the protocol (for IPv4
              IP_RECVERR).  The user should supply a buffer of sufficient
              size.  See cmsg(3) and ip(7) for more information.  The
              payload of the original packet that caused the error is passed
              as normal data via msg_iovec.  The original destination
              address of the datagram that caused the error is supplied via
              msg_name.

              The error is supplied in a sock_extended_err structure:

                  #define SO_EE_ORIGIN_NONE    0
                  #define SO_EE_ORIGIN_LOCAL   1
                  #define SO_EE_ORIGIN_ICMP    2
                  #define SO_EE_ORIGIN_ICMP6   3

                  struct sock_extended_err
                  {
                      uint32_t ee_errno;   // Error number 
                      uint8_t  ee_origin;  // Where the error originated 
                      uint8_t  ee_type;    // Type 
                      uint8_t  ee_code;    // Code 
                      uint8_t  ee_pad;     // Padding 
                      uint32_t ee_info;    // Additional information 
                      uint32_t ee_data;    // Other data 
                      // More data may follow 
                  };

                  struct sockaddr *SO_EE_OFFENDER(struct sock_extended_err *);

              ee_errno contains the errno number of the queued error.
              ee_origin is the origin code of where the error originated.
              The other fields are protocol-specific.  The macro
              SOCK_EE_OFFENDER returns a pointer to the address of the net‐
              work object where the error originated from given a pointer to
              the ancillary message.  If this address is not known, the
              sa_family member of the sockaddr contains AF_UNSPEC and the
              other fields of the sockaddr are undefined.  The payload of
              the packet that caused the error is passed as normal data.

              For local errors, no address is passed (this can be checked
              with the cmsg_len member of the cmsghdr).  For error receives,
              the MSG_ERRQUEUE flag is set in the msghdr.  After an error
              has been passed, the pending socket error is regenerated based
              on the next queued error and will be passed on the next socket
              operation.

       MSG_OOB
              This flag requests receipt of out-of-band data that would not
              be received in the normal data stream.  Some protocols place
              expedited data at the head of the normal data queue, and thus
              this flag cannot be used with such protocols.

       MSG_PEEK
              This flag causes the receive operation to return data from the
              beginning of the receive queue without removing that data from
              the queue.  Thus, a subsequent receive call will return the
              same data.

       MSG_TRUNC (since Linux 2.2)
              For raw (AF_PACKET), Internet datagram (since Linux
              2.4.27/2.6.8), netlink (since Linux 2.6.22), and UNIX datagram
              (since Linux 3.4) sockets: return the real length of the
              packet or datagram, even when it was longer than the passed
              buffer.

              For use with Internet stream sockets, see tcp(7).

       MSG_WAITALL (since Linux 2.2)
              This flag requests that the operation block until the full
              request is satisfied.  However, the call may still return less
              data than requested if a signal is caught, an error or discon‐
              nect occurs, or the next data to be received is of a different
              type than that returned.  This flag has no effect for datagram
              sockets.

   recvfrom()
       recvfrom() places the received message into the buffer buf.  The
       caller must specify the size of the buffer in len.

       If src_addr is not NULL, and the underlying protocol provides the
       source address of the message, that source address is placed in the
       buffer pointed to by src_addr.  In this case, addrlen is a value-
       result argument.  Before the call, it should be initialized to the
       size of the buffer associated with src_addr.  Upon return, addrlen is
       updated to contain the actual size of the source address.  The
       returned address is truncated if the buffer provided is too small; in
       this case, addrlen will return a value greater than was supplied to
       the call.

       If the caller is not interested in the source address, src_addr and
       addrlen should be specified as NULL.

   recv()
       The recv() call is normally used only on a connected socket (see
       connect(2)).  It is equivalent to the call:

           recvfrom(fd, buf, len, flags, NULL, 0);

   recvmsg()
       The recvmsg() call uses a msghdr structure to minimize the number of
       directly supplied arguments.  This structure is defined as follows in
       <sys/socket.h>:

           struct iovec {                    // Scatter/gather array items
               void  *iov_base;              // Starting address 
               size_t iov_len;               // Number of bytes to transfer 
           };                                 
                                              
           struct msghdr {                    
               void         *msg_name;       // Optional address 
               socklen_t     msg_namelen;    // Size of address 
               struct iovec *msg_iov;        // Scatter/gather array 
               size_t        msg_iovlen;     // # elements in msg_iov 
               void         *msg_control;    // Ancillary data, see below 
               size_t        msg_controllen; // Ancillary data buffer len 
               int           msg_flags;      // Flags on received message 
           };

       The msg_name field points to a caller-allocated buffer that is used
       to return the source address if the socket is unconnected.  The call‐
       er should set msg_namelen to the size of this buffer before this
       call; upon return from a successful call, msg_namelen will contain
       the length of the returned address.  If the application does not need
       to know the source address, msg_name can be specified as NULL.

       The fields msg_iov and msg_iovlen describe scatter-gather locations,
       as discussed in readv(2).

       The field msg_control, which has length msg_controllen, points to a
       buffer for other protocol control-related messages or miscellaneous
       ancillary data.  When recvmsg() is called, msg_controllen should con‐
       tain the length of the available buffer in msg_control; upon return
       from a successful call it will contain the length of the control mes‐
       sage sequence.

       The messages are of the form:

           struct cmsghdr {
               size_t cmsg_len;    // Data byte count, including header
                                     (type is socklen_t in POSIX) 
               int    cmsg_level;  // Originating protocol 
               int    cmsg_type;   // Protocol-specific type 
           // followed by
               unsigned char cmsg_data[]; 
           };

       Ancillary data should be accessed only by the macros defined in
       cmsg(3).

       As an example, Linux uses this ancillary data mechanism to pass
       extended errors, IP options, or file descriptors over UNIX domain
       sockets.

       The msg_flags field in the msghdr is set on return of recvmsg().  It
       can contain several flags:

       MSG_EOR
              indicates end-of-record; the data returned completed a record
              (generally used with sockets of type SOCK_SEQPACKET).

       MSG_TRUNC
              indicates that the trailing portion of a datagram was dis‐
              carded because the datagram was larger than the buffer sup‐
              plied.

       MSG_CTRUNC
              indicates that some control data was discarded due to lack of
              space in the buffer for ancillary data.

       MSG_OOB
              is returned to indicate that expedited or out-of-band data was
              received.

       MSG_ERRQUEUE
              indicates that no data was received but an extended error from
              the socket error queue.

RETURN VALUE         top

       These calls return the number of bytes received, or -1 if an error
       occurred.  In the event of an error, errno is set to indicate the
       error.

       When a stream socket peer has performed an orderly shutdown, the
       return value will be 0 (the traditional "end-of-file" return).

       Datagram sockets in various domains (e.g., the UNIX and Internet
       domains) permit zero-length datagrams.  When such a datagram is
       received, the return value is 0.

       The value 0 may also be returned if the requested number of bytes to
       receive from a stream socket was 0.

 */
    int status = recv(this->rhandle, (void *)buf, sizeof(buf), flags);
    if(status < 0 && ( (errno != EAGAIN) || (errno != EWOULDBLOCK) ) ) 
    {
        string s = "[Socket]: Failed to receive - ";
        s += string( strerror(errno) );
        throw runtime_error(s);
    }
    s = string(buf);
    return status; // return nr. of bytes read or -1 (errno should be EAGAIN)
}

void Socket::setTimeout(int sockfd, int seconds)
{
   this->conn_timeout = seconds;
//   struct timeval tv;
//   tv.tv_sec = seconds;
//   tv.tv_usec = 0;
/* Set to non-blocking mode */
   if( !seconds )
   {
       /* Put socket in non-blocking mode */
       int sock_flags = fcntl( sockfd, F_GETFL, 0 );
       fcntl(sockfd, F_SETFL, sock_flags | O_NONBLOCK);
   }

}

bool Socket::isValid() const
{
    return (this->handle != -1);
}

void Socket::setRecvSize(size_t len)
{
    if(len > RECV_BUF_LIM)
        throw runtime_error("[Socket]: Buffer size cannot exceed 4098 bytes");
    rcv_len = len;
}
size_t Socket::RecvSize() const
{
    return rcv_len;
}

void Socket::addToTracking(int sockfd)
{
    /* Add socket to track reading and writing */
    FD_SET(sockfd, &readfds);
    FD_SET(sockfd, &writefds);
    /* Capture max fd: one can use this->handle and this->rhandle */
    maxfd = sockfd;
}

bool Socket::isReadable()
{
    int status = select(maxfd + 1, &readfds, &writefds, NULL, NULL);
    return (status > 0 && ( FD_ISSET(this->handle, &readfds) ||
                            FD_ISSET(this->rhandle, &readfds)) );
}

bool Socket::isWritable()
{
    int status = select(maxfd + 1, &readfds, &writefds, NULL, NULL);
    return (status > 0 && ( FD_ISSET(this->handle, &writefds) ||
                            FD_ISSET(this->rhandle, &readfds)) );
}

