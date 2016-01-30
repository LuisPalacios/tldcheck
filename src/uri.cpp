#include "uri.h"
#include <cstdio>

void Uri::ResetAttributes() {
	_fullUri.clear();
	_isOpaque = false;
	_isHierarchical = false;
	_isRelative = false;
	_scheme.clear();
	_opaque.clear();
	_user.clear();
	_password.clear();
	_host.clear();
	_port.clear();
	_path.clear();
	_query.clear();
	_fragment.clear();
	_domain.clear();
	_tld.clear();
	_isValidURI = false;
	_isBrowsable = false;
	_isValidTLD = false;
}

void Uri::CopyAttributesFrom(const Uri& other) {
	_user = other._user;
	_password = other._password;
	_host = other._host;
	_port = other._port;
	_path = other._path;
	_query = other._query;
	_fragment = other._fragment;
	_domain = other._domain;
	_tld = other._tld;
	_isOpaque = other._isOpaque;
	_isHierarchical = other._isHierarchical;
	_isBrowsable = other._isBrowsable;
	_isValidTLD = other._isValidTLD;
}

Uri::Uri(const std::string& uriString) {
	(void)ParseUri(uriString);
}

Uri::Uri(const std::string& scheme, const std::string& host, const std::string& port, const std::string& path, const std::string& query, const std::string& fragment) {
	std::string uriString = scheme + "://" + host;
	if (!port.empty()) {
		uriString += ":" + port;
	}
	uriString += path;
	if (!query.empty()) {
		uriString += "?" + query;
	}
	if (!fragment.empty()) {
		uriString += "#" + fragment;
	}
	(void)ParseUri(uriString);
}

std::string Uri::ParseUri(const std::string& uriString) {

	std::regex opaque_regex(REGEX_URI_OPAQUE);
	std::regex hierarchical_regex(REGEX_URI_HIERARCHICAL);
	std::regex relative_regex(REGEX_RELATIVE_PATH);
	std::smatch uri_match_result;

	ResetAttributes();

	_fullUri = uriString;

	if (std::regex_match(uriString, uri_match_result, relative_regex)) {
		_isOpaque = false;
		_path = uri_match_result[1].matched ? uri_match_result[1].str() : "";
		_fragment = uri_match_result[2].matched ? uri_match_result[2].str() : "";
		_isRelative = true;
		_isBrowsable = true;
		_isValidURI = true;
		_isHierarchical = true;
		return _fullUri;
	}

	if (!std::regex_match(uriString, uri_match_result, hierarchical_regex)) {
		if (std::regex_match(uriString, uri_match_result, opaque_regex)) {
			if (uri_match_result[1].matched) {
				_scheme = uri_match_result[1].str();
			} else {
				return "";
			}

			_isOpaque = true;
			if (uri_match_result[2].matched) {
				_opaque = uri_match_result[2].str();
			} else {
				return "";
			}

			_isBrowsable = true;
			_isValidURI = true;
			return _fullUri;
		}

		Uri tempUri("https://" + uriString);
		if (tempUri._isValidURI && tempUri._isValidTLD && tempUri._isBrowsable) {
			CopyAttributesFrom(tempUri);
			_isValidURI = false;
			_isRelative = false;
			return _fullUri;
		}

	} else {
		_isHierarchical = true;
		if (uri_match_result[1].matched) {
			_scheme = uri_match_result[1].str();
		} else {
			return "";
		}

		_isOpaque = false;
		_user = uri_match_result[3].matched ? uri_match_result[3].str() : "";
		_password = uri_match_result[4].matched ? uri_match_result[4].str() : "";
		_host = uri_match_result[5].matched ? uri_match_result[5].str() : "";
		_port = uri_match_result[6].matched ? uri_match_result[6].str() : "";
		_path = uri_match_result[7].matched ? uri_match_result[7].str() : "";
		_query = uri_match_result[8].matched ? uri_match_result[8].str() : "";
		_fragment = uri_match_result[9].matched ? uri_match_result[9].str() : "";

		if (!_host.empty()) {
			_domain = _host;
			std::regex ip_regex(Uri::REGEX_IP_ADDRESS);
			if (std::regex_match(_host, ip_regex)) {
				_domain = _host;
			} else if (_host.front() == '.') {
				_domain = _host.substr(1);
			} else {
				size_t lastDot = _host.find_last_of('.');
				if (lastDot != std::string::npos) {
					_tld = _host.substr(lastDot + 1);
					if (TLD_LIST.find(_tld) != TLD_LIST.end()) {
						_isValidTLD = true;
					}
					size_t secondLastDot = _host.find_last_of('.', lastDot - 1);
					if (secondLastDot != std::string::npos) {
						_domain = _host.substr(secondLastDot + 1);
					} else {
						_domain = _host;
					}
				}
			}
		}

		_isValidURI = true;
		if (_tld.empty() || _isValidTLD) {
			_isBrowsable = true;
		}
		return _fullUri;
	}

	return "";
}

bool Uri::IsBrowsable() const {
	return _isBrowsable;
}
