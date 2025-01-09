#include <curl/curl.h>
#include <fstream>
#include <iostream>

size_t writeData(void* ptr, size_t size, size_t nmemb, void* stream) {
    std::ofstream* out = static_cast<std::ofstream*>(stream);
    out->write(static_cast<char*>(ptr), size * nmemb);
    return size * nmemb;
}

void downloadOSMData(const std::string& url, const std::string& output_file) {
    CURL* curl = curl_easy_init();
    if (curl) {
        std::ofstream file(output_file, std::ios::binary);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Error: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
}

int main() {
    std::string url = "https://download.geofabrik.de/europe/germany/berlin-250107.osm.pbf";
    std::string output_file = "berlin.osm.pbf";
    downloadOSMData(url, output_file);
    return 0;
}
