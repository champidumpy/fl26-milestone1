#include "aiws/corpus_index.hpp"
#include "aiws/text_processor.hpp"
#include <unordered_set>
#include <unordered_map>
namespace aiws {

CorpusIndex::CorpusIndex(const std::vector<Chunk>& chunks) {
    build(chunks);
}

void CorpusIndex::build(const std::vector<Chunk>& chunks) {
    postings_.clear();
    chunk_by_id_.clear();
    for(std::size_t i = 0; i < chunks.size(); i++) {
        chunk_by_id_[chunks[i].id] = i;
        auto terms = TextProcessor::terms(chunks[i].text);
        std::unordered_map<std::string, std::size_t> termfreq;
        for(const auto& token : terms) {
            termfreq[token]++;
        }
        for(const auto& entry : termfreq) {
            postings_[entry.first].push_back({i, entry.second});
        }
    }
    // TODO: build the searchable index from the supplied chunks.
}

std::size_t CorpusIndex::document_frequency(
    const std::string& normalized_term) const noexcept {
    auto it = postings_.find(normalized_term);
    if(it == postings_.end()) {
        return 0;
    }
    // TODO: return how many chunks contain the requested term.
    return it->second.size();
}

std::size_t CorpusIndex::term_frequency(
    const std::string& normalized_term,
    const std::string& chunk_id) const noexcept {
        auto it = postings_.find(normalized_term);
        if(it == postings_.end()) {
            return 0;
        }
        auto chunk_it = chunk_by_id_.find(chunk_id);
        if(chunk_it == chunk_by_id_.end()) {
            return 0;
        }
        std::size_t wanted_index = chunk_it->second;
        for(const auto& posting : it->second) {
            if(posting.chunk_index == wanted_index) {
                return posting.frequency;
            }
        }
    // TODO: return the requested term's frequency in the specified chunk.
    return 0;
}

const std::vector<CorpusIndex::Posting>* CorpusIndex::postings(
    const std::string& normalized_term) const noexcept {
    auto it = postings_.find(normalized_term);
    if(it == postings_.end()) {
        return nullptr;
    }
    // TODO: return the postings associated with the requested term.
    return  &it->second;
}

const Chunk* CorpusIndex::find_chunk(
    const std::vector<Chunk>& chunks,
    const std::string& chunk_id) const noexcept {
        auto  it = chunk_by_id_.find(chunk_id); 
        if(it == chunk_by_id_.end()) {
            return nullptr;
        }

    // TODO: find the chunk identified by the requested chunk ID.
    return &chunks[it->second];
}

std::size_t CorpusIndex::chunk_index(const std::string& chunk_id) const {
    // TODO: return the stored index of the requested chunk ID.

    auto it = chunk_by_id_.find(chunk_id);
    if(it == chunk_by_id_.end()) {
        return 0;
    }
    return it->second;
}

}  // namespace aiws
