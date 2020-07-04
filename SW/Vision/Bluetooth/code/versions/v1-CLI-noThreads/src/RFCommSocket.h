/**
 * @file RFCommSocket.h
 * @author Jose Pires
 * @date 03-06-2019
 *
 * @brief Interface for a RFCOMM socket
 *
 * This is also an abstract class, that derives from Socket.
 * - It specializes Socket by constructing the socket with the appropriate 
 *   parameters (always fixed) and adding an addressing structure, which is
 *   specific to the type of protocol
 * - It is abstract because it must be connected to be useful; thus the need 
 *   for specialization
 */
#ifndef RFCOMMSocket_H
#define RFCOMMSocket_H

#include "Socket.h"
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

using std::string;

class RFCommSocket : public Socket {
protected:
/* Address struct: Specific to each type of socket */
    sockaddr_rc loc_addr; /**< RFCommSocket local address */ 
    sockaddr_rc rem_addr; /**< RFCommSocket remote address */
    static constexpr sa_family_t bd_family = AF_BLUETOOTH;
    static constexpr bdaddr_t bd_any = {0, 0, 0, 0, 0, 0};
    static constexpr bdaddr_t bd_local = {0, 0, 0, 0xff, 0xff, 0xff};
    string addrInfo(const sockaddr_rc *addr) const;
public:
   RFCommSocket(); 
/* Used to create abstract class; it must be declared like this, but must also
 * be implemented, so subclasses can destroy the base class */
   virtual ~RFCommSocket() = 0; 
/**
 * @brief Checks if a RFCOMM port is valid
 * @param port: port to be checked
 * @return true if valid; false otherwise.
 *
 * RFCOMM ports are valid in range 1-30.
 *
 * Subclasses cannot override this function.
 */
   bool isPortValid(int port) const final;

/**
 * @brief Retrieves the address information in human-readable form
 * @return string containing the RFCOMM address and port
 *
 * Exceptions thrown:
 * - 1. If socket is not connected, it's not possible to obtain its address
 * info.
 */
   string localAddrInfo() const;
   string remoteAddrInfo() const;
}; 

#endif // RFCOMMSocket_H
