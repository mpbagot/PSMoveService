// -- includes -----
#include "BluetoothQueries.h"
#include "ServerLog.h"
#include "ServerUtility.h"
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <assert.h>
#include <sstream>
#include <iomanip>


//-- Queries -----
bool bluetooth_get_host_address(std::string &out_address)
{
  static std::string x_cachedHostBluetoothAddress= "";
  bool bSuccess = true;
  bdaddr_t bdaddr;
  int dev_id, sock;
  char addr[17];

  if (x_cachedHostBluetoothAddress.length() == 0)
  {
    // Try to get the id of the first available adapter
    dev_id = hci_get_route(NULL);
    sock = hci_open_dev(dev_id);
    if (dev_id < 0 || sock < 0)
    {
      SERVER_LOG_ERROR("bluetooth_get_host_address") << "Failed to find a bluetooth radio";
      bSuccess = false;
    }
    else
    {
      SERVER_LOG_INFO("bluetooth_get_host_address") << "Found a bluetooth radio";
    }

    // Try to get the bluetooth device address. Returns 0 on success
    memset(&bdaddr, 0, sizeof(bdaddr));
    if (hci_devba(dev_id, &bdaddr))
    {
      SERVER_LOG_ERROR("bluetooth_get_host_address") << "Failed to get radio host address";
      bSuccess = false;
    }
    else
    {
      SERVER_LOG_INFO("bluetooth_get_host_address") << "Retrieved radio address";
      // Convert binary address to char* string
      ba2str(&bdaddr, addr);
      // Then, assign the char* data to the std::string out_address
      out_address.assign(addr);
      x_cachedHostBluetoothAddress= out_address;
    }
  }
  else
  {
    SERVER_LOG_INFO("bluetooth_get_host_address") << "Using cached radio address";
    out_address= x_cachedHostBluetoothAddress;
    bSuccess= true;
  }

  return bSuccess;

}
