#ifndef URI_H
#define URI_H

#include <string>
#include <regex>
#include "iana-tlds.h"

class Uri {
private:
	std::string _fullUri;
	bool _isOpaque;
	bool _isHierarchical;
	bool _isRelative;
	std::string _scheme;
	std::string _opaque;
	std::string _user;
	std::string _password;
	std::string _host;
	std::string _port;
	std::string _path;
	std::string _query;
	std::string _fragment;
	std::string _domain;
	std::string _tld;
	bool _isValidURI;
	bool _isBrowsable;
	bool _isValidTLD;

	void ResetAttributes();
	void CopyAttributesFrom(const Uri& other);
	std::string ParseUri(const std::string& uriString);

	static constexpr const char* REGEX_URI_OPAQUE = R"(^([^:]+):([^\/].*)$)";
	static constexpr const char* REGEX_URI_HIERARCHICAL = R"(^([^:]+):\/\/(([^:@]*)(?::([^:@]*))?@)?([^:\/?#]*)(?::(\d+))?([^?#]*)(?:\?([^#]*))?(?:#(.*))?$)";
	static constexpr const char* REGEX_RELATIVE_PATH = R"(^(\.?\.?/|[^:?#]+/)[^?#]*(?:#(.*))?$)";
	static constexpr const char* REGEX_DOMAIN = R"(^([^\.]+|\d{1,3}(\.\d{1,3}){3}|[^\.]+\.[^\.]+)$)";
	static constexpr const char* REGEX_IP_ADDRESS = "^\\d{1,3}(\\.\\d{1,3}){3}$";

public:
	Uri() = default;
	explicit Uri(const std::string& uriString);
	Uri(const std::string& scheme, const std::string& host, const std::string& port, const std::string& path, const std::string& query, const std::string& fragment);
	bool IsBrowsable() const;
};

#endif
