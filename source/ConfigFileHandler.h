#ifndef CONFIG_FILE_HANDLER_H
#define CONFIG_FILE_HANDLER_H
#include <string>
#include <vector>

class ConfigFileHandler {
public:
    ConfigFileHandler(const std::string &filename);

    ConfigFileHandler();

    bool SaveData(const std::string data);

    bool LoadData(std::string &data) const;

private:
    std::string GetDocumentsDirectory() const;

    bool CreateDir(const std::string &path) const;

    bool CreateDir(const std::wstring &path) const;

    bool WriteToFile(const std::vector<std::vector<std::string> > &data) const;

    std::string m_filename = "config.conf";
};
#endif
