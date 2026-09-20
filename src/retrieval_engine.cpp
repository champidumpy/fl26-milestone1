#include "aiws/retrieval_engine.hpp"
#include "aiws/text_processor.hpp"
#include <unordered_set>
#include <cmath>
#include <algorithm>
#include <stdexcept>
namespace aiws {

double RetrievalEngine::canonical_score(double value) {
    // TODO: return the score in the required canonical form.
    return std::round(value * 100) / 100;
}

std::vector<SearchResult> RetrievalEngine::search(const std::string& query,
                                                  int k,
                                                  const std::vector<Chunk>& chunks,
                                                  const CorpusIndex& index) const {
    // TODO: return the  ranked search results for the requested query.
    if(k < 0) {
        throw std::invalid_argument("k needs to be positive");
    }
    if(k== 0){
        return{};
    }
    auto terms = TextProcessor::terms(query);
    std::unordered_set<std::string> uniquet(terms.begin(), terms.end());
    if(uniquet.empty()) {
        return {};
    }
    std::vector<SearchResult> results;
    std::size_t chunkcount = chunks.size();
    
    for(std::size_t i = 0; i < chunkcount; ++i) {
        const Chunk& chunk = chunks[i];
        double score = 0.0;
        std::size_t match = 0;
        for(const auto& term : uniquet) {
            std::size_t tf = index.term_frequency(term, chunk.id);
            if(tf == 0) {
                continue;
            }

            std::size_t df = index.document_frequency(term);
            double idf = std::log((static_cast<double>(chunkcount) + 1.0) / (1.0 + static_cast<double>(df))) + 1.0;
                score += static_cast<double>(tf) * idf;
                match++;

        }
        if (match == 0) {
            continue;
        }

        double coverage =  static_cast<double>(match) / static_cast<double>(uniquet.size());
        score += coverage;
        SearchResult result;
        result.document_id = chunk.document_id;
        result.chunk_id = chunk.id; 
        result.chunk_sequence = chunk.sequence;
        result.text = chunk.text;
        result.score = canonical_score(score);
        result.matched_terms = match;
        results.push_back(result);

    }
    std::sort(results.begin(), results.end(), [](const SearchResult& a, const SearchResult& b) {
        if(a.score != b.score) {
            return a.score > b.score;
        }
        if(a.document_id != b.document_id) {
            return a.document_id < b.document_id;
        }
        return a.chunk_sequence < b.chunk_sequence;
    });
    if(static_cast<std::size_t>(k) < results.size()) {
        results.resize(static_cast<std::size_t>(k));
    }
    return results;
}

}  // namespace aiws

