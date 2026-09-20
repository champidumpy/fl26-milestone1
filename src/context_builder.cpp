#include "aiws/context_builder.hpp"
#include "aiws/text_processor.hpp"
namespace aiws {

std::vector<ContextItem> ContextBuilder::build(const std::vector<SearchResult>& ranked, std::size_t token_budget) const {
    // TODO: assemble ranked results into context items within the supplied token budget.
    std::vector<ContextItem> con;
    std::size_t totaltokens = 0;
    for(const auto& result : ranked) {
        auto tokens = TextProcessor::terms(result.text);
        std::size_t tokenamount = tokens.size();
        if(totaltokens + tokenamount <= token_budget) {
            con.push_back({result.chunk_id, result.document_id, result.chunk_sequence,result.text,tokenamount,result.score, false});
            totaltokens += tokenamount;
        }
        else{
            if(totaltokens >= token_budget) {
                break;
            }
            std::size_t remaining = token_budget - totaltokens;
            con.push_back({result.chunk_id, result.document_id, result.chunk_sequence,TextProcessor::join(tokens,0,remaining),remaining,result.score, true});
            break;
        }
    }

    return con;
}

}  // namespace aiws
