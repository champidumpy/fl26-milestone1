#include "aiws/text_processor.hpp"
#include <cctype> 
namespace aiws {

std::vector<TokenInfo> TextProcessor::tokenize(const std::string& text) {
    // TODO: produce normalized tokens with source and paragraph information.
    std::vector<TokenInfo> tokens;
    std::size_t paragraph = 0;
    std::size_t start = 0;
    std::string currentToken;
    bool lastnewLine = false;

    for (std::size_t i = 0; i < text.size(); ++i) {
    
        unsigned char c = text[i];
        if (c == '\n') {
            if(lastnewLine) {
                paragraph++;
            }
            lastnewLine = true;
        } else {
            lastnewLine = false;
        }
        if(std::isalpha(c)) {
            if(currentToken.empty()) {
                start = i;
            }
            currentToken += c;
        } else if(std::isdigit(c)) {
            if(currentToken.empty()) {
                start = i;
            }
            currentToken += c;
        } 
        else {
            if(!currentToken.empty()) {
                tokens.push_back(TokenInfo{currentToken, start, i, paragraph});
                currentToken.clear();
            }
        }
    }
    if(!currentToken.empty()) {
        tokens.push_back(TokenInfo{currentToken, start, text.size(), paragraph});
    }
    return tokens;
}

std::vector<std::string> TextProcessor::terms(const std::string& text) {
    // TODO: return the normalized terms represented by the input text.
    auto tokens = tokenize(text);
    std::vector<std::string> output;
    for (const auto& token : tokens) {
        output.push_back(token.token);
    }
    return output;
}

std::string TextProcessor::normalize(const std::string& text) { 
    // TODO: return the normalized form of the input text.
    auto tokens = tokenize(text);   
    return join(tokens, 0, tokens.size());
}

std::string TextProcessor::join(const std::vector<TokenInfo>&,
                                std::size_t,
                                std::size_t) {
    // TODO: join the requested token range into normalized text.
    std::string output;
    for(std::size_t i = begin; i < end && i < tokens.size(); i++) {
        if(!output.empty()) {
            output += " ";
        }
        output += tokens[i].token;
    }
    return output;
}

std::string TextProcessor::join(const std::vector<std::string>& tokens,
                                std::size_t begin,
                                std::size_t end) {
    // TODO: join the requested term range into normalized text.
    std::string output;
    for(std::size_t i = begin; i < end && i < tokens.size(); i++) {
        if(!output.empty()) {
            output += " ";
        }
        output += tokens[i];
    }
    return output;
}

}  // namespace aiws
