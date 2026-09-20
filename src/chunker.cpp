#include "aiws/chunker.hpp"
#include "aiws/text_processor.hpp"
#include <stdexcept>

namespace aiws {

Chunker::Chunker(ChunkingPolicy policy) : policy_(policy) {
    if (policy_.max_tokens == 0 || policy_.overlap >= policy_.max_tokens ||
        policy_.paragraph_window > policy_.max_tokens) {
        throw std::invalid_argument("invalid chunking policy");
    }
}

std::vector<Chunk> Chunker::chunk(const Document& document, std::size_t document_order  ) const {
    // TODO: produce deterministic, source-attributed chunks for the supplied document.
    std::vector<Chunk> chunks;
    auto tokens = TextProcessor::tokenize(document.text());
    if (tokens.empty()) {
        return chunks;
    }
    std::size_t start = 0;
    std::size_t se = 0;
    while(start < tokens.size()) {
        std::size_t end = std::min(start + policy_.max_tokens, tokens.size());
        std::size_t e = end; 
        if(end - start == policy_.max_tokens) {
            std::size_t window = (end>= policy_.paragraph_window) ? end - policy_.paragraph_window : start;
        }
        for(std::size_t i = end; i > start; --i) {
            if(i < tokens.size() && tokens[i-1].paragraph != tokens[i-1].paragraph) {
                end = i;
                break;
            }
        }
        Chunk c;
        c.document_id = document.id();
        c.document_order = document.order();
        c.sequence = chunks.size();
        c.id = document.id() + "#" + std::to_string(c.sequence);
        c.token_count = end - start;
        c.text = TextProcessor::join(tokens, start, end);
        c.source_begin = tokens[start].begin;
        c.source_end = tokens[end-1].end;
        chunks.push_back(c);
        if(end >= tokens.size()) {
            break;
        }
        if(end < policy_.overlap) {
            start = 0;
        } else {
            start = end - policy_.overlap;
        }
    }

    return chunks;
}

}  // namespace aiws
