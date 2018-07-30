#include <iostream>
#include <sstream>
#include <miniupnp/miniupnpc/miniupnpc.h>
#include <miniupnp/miniupnpc/upnpcommands.h>


class upnp
{
public:
	upnp() :
		get_valid_IGD_result(-1),
		is_mapped(false)
    {
    }

	~upnp()
	{
		if (is_mapped)
			UPNP_DeletePortMapping(urls->controlURL, datas->first.servicetype, ext_port, "TCP", nullptr);
		if (get_valid_IGD_result > 0)
			FreeUPNPUrls(urls);
		delete urls;
		delete datas;
		delete ext_port;
	}

    void port_mapping(int in_port_a, int ex_port_a)
    {
		std::cout << "upnpDiscover" << std::endl;

        int discover_err = 0;
        struct UPNPDev * device_list = upnpDiscover(2000, nullptr, nullptr, 0, 0, 2, &discover_err);

		auto dev = device_list;
		while (dev)
		{
			std::cout << "UPnP device:" << dev->descURL << "[st:" << dev->st << "]" << std::endl;
			dev = dev->pNext;
		}
		
		std::cout << "UPNP_GetValidIGD" << std::endl;

		urls = new UPNPUrls;
		datas = new IGDdatas;
		char lan_addr[64];
        get_valid_IGD_result = UPNP_GetValidIGD(device_list, urls, datas, lan_addr, sizeof(lan_addr));

        if(get_valid_IGD_result == 1)
        {
			//UPNP_GetExternalIPAddress 
            char external_ip_addr[16];
			int error = UPNP_GetExternalIPAddress(urls->controlURL, datas->first.servicetype, external_ip_addr);
			std::cout << "urls->controlURL:" << urls->controlURL << ", datas->first.servicetype" << datas->first.servicetype << std::endl;
			if (error)
			{
				std::cout << "UPNP_GetExternalIPAddress fail." << std::endl;
			}
			else
			{
				std::cout << "ExternalIPAddress: " << external_ip_addr << std::endl;
			}

			//UPNP_AddPortMapping	
			ext_port = new char;		
			sprintf(ext_port, "%d", ex_port_a);
			char * in_port = new char;
			sprintf(in_port, "%d", in_port_a);
			error = UPNP_AddPortMapping(urls->controlURL, datas->first.servicetype, ext_port, in_port, lan_addr, "gx-upnp", "TCP", nullptr, nullptr);
			if (error)
			{
				std::cout << "UPNP_AddPortMapping fail." << std::endl;
			}
			else
			{
				is_mapped = true;
				std::cout << "add port mapping on external port:" << ex_port_a << std::endl;
			}
			delete in_port;
        }
        else
        {
            std::cout << "No valid UPnP IGD found, result:" << get_valid_IGD_result << std::endl;
        }

		freeUPNPDevlist(device_list);
    }

	void show_port_mapping()
	{
		if (get_valid_IGD_result > 0)
		{
			std::cout << "urls->controlURL:" << urls->controlURL << ", datas->first.servicetype" << datas->first.servicetype << std::endl;
			int i = 0;
			do
			{
				char extPort[16];
				char intClient[16];
				char intPort[6];
				char protocol[4];
				char desc[80];
				char enabled[4];
				char rHost[64];
				char duration[16];
				int r = UPNP_GetGenericPortMappingEntry(urls->controlURL, datas->first.servicetype, toString(i).c_str(), extPort, intClient, intPort, protocol, desc, enabled, rHost, duration);
				if (r == UPNPCOMMAND_SUCCESS)
				{
					std::cout << "desc:" << desc << ", host:" << rHost << ", external port:" << extPort << ", map to:" << intClient << ":" << intPort << std::endl;
				}
			} while (i++ < 9000);
		}
	}

	template <class _T>
	inline std::string toString(_T const& _t)
	{
		std::ostringstream o;
		o << _t;
		return o.str();
	}

private:
	struct UPNPUrls * urls;
	struct IGDdatas * datas;
	char * ext_port;
	int get_valid_IGD_result;
	bool is_mapped;
};