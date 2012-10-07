#include <bets42/deepthgt/betfair/Requests.hpp>
#include <bets42/arthur/memory.hpp>
#include <bets42/deepthgt/Exception.hpp>
#include <pugixml/pugixml.hpp>
#include <functional>
#include <memory>

namespace
{
	const char* const LOGIN_REQ_XML = "<hello>";
	const char* const LOGOUT_REQ_XML = "<hello>";
	const char* const KEEP_ALIVE_REQ_XML = "<hello>";

	std::unique_ptr<pugi::xml_document> createRequest(const std::string& xml)
	{
		auto req(bets42::arthur::make_unique<pugi::xml_document>());

		const pugi::xml_parse_result result(req->load_buffer(xml.data(), xml.size()));

		if(! result)
		{
			throw bets42::deepthgt::Exception(__FILE__, __LINE__)
			        << "Failed to create request, error=" << result.description();
		}

		return req;
	}
}

std::ostream& bets42::deepthgt::betfair::createLoginRequest(
    std::ostream& stream,
    const std::string& username,
    const std::string& password,
    const unsigned productID)
{
	static auto request = createRequest(LOGIN_REQ_XML);
	static auto fields = std::make_tuple
	                     (
	                         request->select_single_node("xpath").node(),
	                         request->select_single_node("xpath").node(),
	                         request->select_single_node("xpath").node()
	                     );

	std::get<0>(fields).set_value(username.c_str());
	std::get<1>(fields).set_value(password.c_str());
	std::get<2>(fields).set_value(std::to_string(productID).c_str());

	request->save(stream);

	return stream;
}

std::ostream& bets42::deepthgt::betfair::createLogoutRequest(
    std::ostream& stream,
    const std::string& sessionID)
{
	static auto request = createRequest(LOGOUT_REQ_XML);
	static auto fields = std::make_tuple
	                     (
	                         request->select_single_node("xpath").node()
	                     );

	std::get<0>(fields).set_value(sessionID.c_str());

	request->save(stream);

	return stream;
}

std::ostream& bets42::deepthgt::betfair::createKeepAliveRequest(
    std::ostream& stream,
    const std::string& sessionID)
{
	static auto request = createRequest(KEEP_ALIVE_REQ_XML);
	static auto fields = std::make_tuple
	                     (
	                         request->select_single_node("xpath").node()
	                     );

	std::get<0>(fields).set_value(sessionID.c_str());

	request->save(stream);

	return stream;
}

