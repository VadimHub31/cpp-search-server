#include "document.h"

Document::Document() = default;

Document::Document(int id_, double relevance_, int rating_): id(id_), relevance(relevance_), rating(rating_) {}

std::ostream& operator<<(std::ostream& output, const Document& document) {
    output << "{ document_id = "s << document.id << ", relevance = "s 
           << document.relevance << ", rating = "s << document.rating << " }"s;
    return output;  
}