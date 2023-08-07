#include "jwt_verify.h"

int add(int a, int b) { return a + b; }

std::string json;

long long getCharLength(const char* str) {
	long long length = 0;
	for (long long i = 0;; i++) {
		length++;
	if (str[i] == '\0') break;
	}
    length++;
	return length;
}

char* clearEnter(const char* src) {
    auto length = getCharLength(src);
    char* ans = (char*)malloc((length + 1) * sizeof(char));
    int record = 0;
    for(int i =0; i< length; i++) {
        if (src[i] == 10)
            continue;
        ans[i] = src[i];
        record++;
    }
    ans[record] = 0;
    return ans;
}

char* constCharMapToChar(const char* tempStr) {
	auto length = getCharLength(tempStr);
	char* ans = (char*)malloc((length + 1) * sizeof(char));
	for (long long i = 0; i < length; i++) {
		ans[i] = tempStr[i];
	}
	return ans;
}

char* verifyToken(const char* token, const char* publicKey) {
    std::string tokenStr(token);
    std::string publicKeyStr(publicKey);
    std::cout << "begin to decode token"<<std::endl;
	try {
		auto decoded = jwt::decode(tokenStr);
		std::cout << "end to decode token"<<std::endl;
        
		std::cout << "begin to get verify change, publicKey: "<< publicKeyStr << std::endl;
		auto verifier = jwt::verify().allow_algorithm(jwt::algorithm::ps256(publicKeyStr, "", "", "")).with_issuer("dbh");
		std::cout << "end to get verify" << std::endl;

		try {
			verifier.verify(decoded);
			return constCharMapToChar("Success");
		} catch (const std::exception& ex) {
			return constCharMapToChar(std::string(ex.what()).c_str());
		}
	} catch (const std::exception& ex) {
		return constCharMapToChar(std::string(ex.what()).c_str());
	}
}

char* decodeTokenToJson(const char* token) {
	try {
		auto decoded = jwt::decode(token);
        auto header = decoded.get_header().c_str();
		auto payload = decoded.get_payload().c_str();
		//char* sign = clearEnter(decoded.get_signature().c_str());
		json = "{\"header\": " + std::string(header) + ",\"payload\":" + std::string(payload) + "}";
		auto tempStr = json.c_str();
		auto ans = constCharMapToChar(std::string("{\"success\": true, \"data\":" + std::string(tempStr) + "}").c_str());
		std::cout << "C++ decodeToken Success change: \n" << ans << "\n" << std::endl;
		return ans;
	} catch (std::exception& e) {
		std::cout << "c++ decode Token exception change" << std::endl;
		return constCharMapToChar(std::string("{\"success\": false, \"reason\": " + std::string(e.what()) + "}").c_str());
	}
}
