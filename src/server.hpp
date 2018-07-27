#include <iostream>

#include <boost/asio.hpp>

namespace ba = boost::asio;
namespace bi = boost::asio::ip;

class server
{
public:
	server():
		_acceptor(_io_service)
	{
	}

	void start(std::string addr_a, uint16_t port_a)
	{
		boost::system::error_code ec;
		bi::address addr = bi::address::from_string(addr_a, ec);
		if (ec)
		{
			std::cerr << "invalid addr" << std::endl;
		}
		bi::tcp::endpoint endpoint(addr, port_a);

		try
		{
			_acceptor.open(endpoint.protocol());
			_acceptor.set_option(ba::socket_base::reuse_address(true));
			_acceptor.bind(endpoint);
			_acceptor.listen();
		}
		catch (std::exception const & e)
		{
			std::cerr << "acceptor listen fail, endpoint:" <<  endpoint << ", message:" << e.what() << std::endl;
			return;
		}

		std::cout << "acceptor listen on" << endpoint << std::endl;

		do_accept();

		_io_thread = std::thread([this]() {
			_io_service.run();
		});
	}

	void do_accept()
	{
		std::shared_ptr<bi::tcp::socket> socket(std::make_shared<bi::tcp::socket>(_io_service));
		_acceptor.async_accept(*socket, [socket, this](boost::system::error_code ec)
		{
			if (ec)
			{
				std::cerr << "async_accept error!!! " << socket->remote_endpoint() << std::endl;
			}
			else
			{
				std::cout << "async_accept, remote endpoint:" << socket->remote_endpoint() << std::endl;
			}

			socket->close();

			do_accept();
		});
	}

private:
	ba::io_service _io_service;
	bi::tcp::acceptor _acceptor;
	std::thread _io_thread;
};