
#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace HSP
{
    class Response
    {
    private:
        std::vector<uint8_t> m_data;
        std::unordered_map<std::string, std::string> m_headers;
        // ...signature
    public:
        Response() = default;
        ~Response() = default;
    public:
        inline const uint8_t* RawData() const { return m_data.data(); }
        inline unsigned int PayloadSize() const { return m_data.size(); }
        inline const std::unordered_map<std::string, std::string>& GetHeaders() const { return m_headers; }
        template<typename InputIt>
        inline void InsertData(InputIt first, InputIt last)
            { m_data.insert(m_data.begin(), first, last); }
        inline void AddHeader(std::pair<std::string, std::string> header)
            { m_headers.insert(header); }
        inline std::string GetHeader(std::string key)
        {
            if (m_headers.find(key) != m_headers.end())
            {
                return m_headers.at(key);
            }
            return std::string("");
        }
    };
}

