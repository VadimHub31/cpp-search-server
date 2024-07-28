#pragma once
#include "search_server.h"
#include <deque>

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);

    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    int GetNoResultRequests() const;

private:
    struct QueryResult {
        std::string request;
        bool is_empty;
    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    int empty_requests_ = 0;
    int current_time_ = 0;
    const SearchServer& search_server_;
};

template <typename DocumentPredicate>
    std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        ++current_time_;
        if (current_time_ > 1440) {
            requests_.pop_front();
            --empty_requests_;
        }
        QueryResult query_result;
        query_result.request = raw_query;
        auto result = search_server_.FindTopDocuments(raw_query, document_predicate);
        if (result.empty()) {
            query_result.is_empty = true;
        }
        else {
            query_result.is_empty = false;
        }
        
        requests_.push_back(query_result);
        
        if (query_result.is_empty) {
            ++empty_requests_;
        }

        return result;
    }