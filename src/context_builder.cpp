#include "aiws/context_builder.hpp"

namespace aiws {

std::vector<ContextItem> ContextBuilder::build(const std::vector<SearchResult>& ranked, std::size_t token_budget) const {
    // TODO: assemble ranked results into context items within the supplied token budget.
    std::vector<ContextItem> con;
    std::size_t totaltokens = 0;
    for(const auto& result : ranked) {
        if(totaltokens >= token_budget) {
            break;
        }   
        std::size_t tokenamount = result.token_count;
        if(totaltokens + tokenamount <= token_budget) {
            con.push_back({result.document_id, result.chunk_id, tokenamount, false});
            totaltokens += tokenamount;
        } else {
            std::size_t remaining = token_budget - totaltokens;
            if(remaining > 0) {
                con.push_back({result.document_id, result.chunk_id, remaining, true});
                
            }
            break;

    }
}

    return con;
}

}  // namespace aiws
