#include <iostream>
#include <uv.h>


int main(int argc, char* argv[])
{
	char buf[512];
	uv_interface_address_t *info;
	int count, i;
	uv_interface_addresses(&info, &count);
	i = count;
	std::cout << "Number of interfaces: " << count << "\n";
	while (i--)
	{
		uv_interface_address_t inter_face = info[i];
		std::cout << "Name: " << inter_face.name << "\tInternal? " << (inter_face.is_internal ? "Yes" : "No") << "\n";
		if (inter_face.address.address4.sin_family == AF_INET)
		{
			uv_ip4_name(&inter_face.address.address4, buf, sizeof(buf));
			std::cout << "IPv4 address: " << buf << std::endl;
		}
		else if (inter_face.address.address4.sin_family == AF_INET6)
		{
			uv_ip6_name(&inter_face.address.address6, buf, sizeof(buf));
			std::cout << "IPv6 address: " << buf << std::endl;
		}
	}
	uv_free_interface_addresses(info, count);
	return 0;
}