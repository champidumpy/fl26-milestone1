#include "aiws/processing_core.hpp"
#include "aiws/text_processor.hpp"
#include "aiws/retrieval_engine.hpp"
#include "aiws/context_builder.hpp"
#include "aiws/chunker.hpp"
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
    ChunkingPolicy policy{120,20,20};
    Chunker chunker(policy);
    std::unordered_set<std::string> seen;
    for(const auto& doc : workspace.documents()) {
        if(seen.find(doc.id()) != seen.end()) {
            throw std::invalid_argument("same doc id");
        }
        auto c = chunker.chunk(doc, newchunks.size());
        newchunks.insert(newchunks.end(), c.begin(), c.end());
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
