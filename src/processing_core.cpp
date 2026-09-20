#include "aiws/processing_core.hpp"
#include "aiws/text_processor.hpp"
#include "aiws/retrieval_engine.hpp"
#include "aiws/context_builder.hpp"
#include "aiws/chunker.hpp"
#include <unordered_set>
#include <stdexcept>
namespace aiws {

struct ProcessingCore::Impl {
    // TODO: define the internal state used by the processing core.
    std::vector<Chunk> chunks;
    CorpusIndex index;
};

ProcessingCore::ProcessingCore() : impl_(std::make_unique<Impl>()) { }

ProcessingCore::~ProcessingCore() = default;

ProcessingCore::ProcessingCore(ProcessingCore&&) noexcept = default;

ProcessingCore& ProcessingCore::operator=(ProcessingCore&&) noexcept = default;

std::string ProcessingCore::normalize(const std::string& text) {
    // TODO: return the normalized form of the input text.

    return TextProcessor::normalize(text);
}

void ProcessingCore::rebuild(const Workspace& workspace) {
    std::vector<Chunk> newchunks;
    std::unordered_set<std::string> document_ids;
    std::size_t document_order = 0;
    for(const auto& document : workspace.documents()) {
        if(!document_ids.insert(document.id()).second) {
            throw std::invalid_argument("same document id");
        }
        Chunker chunker({kMaxChunkTokens, kChunkOverlap, kParagraphPreferenceWindow});
        auto document_chunks = chunker.chunk(document, document_order);
        newchunks.insert(newchunks.end(), document_chunks.begin(), document_chunks.end());
        document_order++;
    }
    CorpusIndex newindex(newchunks);
    impl_->chunks = std::move(newchunks);
    impl_->index = std::move(newindex);
    // TODO: rebuild the processing state from the workspace.
}

const std::vector<Chunk>& ProcessingCore::chunks() const noexcept {

//TODO: return the chunks currently stored by the processing core.
    return impl_->chunks;
}

std::size_t ProcessingCore::chunk_count() const noexcept {
    // TODO: return the number of stored chunks.
    return impl_->chunks.size();
}

std::size_t ProcessingCore::document_frequency(const std::string& term) const {
    // TODO: return the document frequency for the requested term.
    if(TextProcessor::terms(term).size() > 1){
        throw std::invalid_argument("term normalize to 1 token");
    }
    return impl_->index.document_frequency(normalize(term));
}

std::size_t ProcessingCore::term_frequency(const std::string& term, 
                                           const std::string& chunk_id) const {
    // TODO: return the term frequency for the requested chunk.
    return impl_->index.term_frequency(normalize(term), chunk_id);
}

std::vector<SearchResult> ProcessingCore::search(const std::string& query, int k) const {
    // TODO: return the ranked results for the requested query.
    RetrievalEngine engine;
    return engine.search(query, k, impl_->chunks, impl_->index);
}

std::vector<ContextItem> ProcessingCore::build_context(const std::string& query,
                                                       int k,
                                                       std::size_t token_budget) const {
    // TODO: build bounded context for the requested query.
    auto results = search(query, k);
    ContextBuilder builder;
    return builder.build(results, token_budget);
}

}  // namespace aiws
