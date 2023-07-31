#include <iostream>
#include <filesystem>
#include <fstream> // Для работы с файлами
#include <string>
#include <locale> // Для работы с локалью
#include <sstream> // Для работы с std::istringstream

namespace fs = std::filesystem;

// Функция, которая рекурсивно считает общий размер директории
uintmax_t calculateTotalSize(const fs::path& path, bool inBytes) {
    if (fs::exists(path) && fs::is_directory(path)) {
        uintmax_t totalSize = 0;
        bool isEmptyDirectory = true;
        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            if (fs::is_regular_file(entry)) {
                totalSize += fs::file_size(entry);
                isEmptyDirectory = false;
            }
        }

        if (isEmptyDirectory) {
            return inBytes ? 0 : 1; // Размер пустой директории: 0 байт или 1 блок по 512 байт
        }

        if (!inBytes) {
            totalSize = (totalSize + 511) / 512;
        }
        return totalSize;
    }
    return 0;
}


int main() {
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());

    std::wstring input;
    std::wcout << L"Введите данные для поиска, для поиска по файлу перед названием указать @имя файла (для завершения введите 'exit'): ";
    std::getline(std::wcin, input);

    while (input != L"exit") {
        bool showInBytes = false;
        bool showEachFile = false;
        bool showTotalSize = false;
        bool showOnlyTotalSize = false;
        //может использовать одну переменную, вместо 4 переменных.

        // Проверяем, начинается ли ввод с символа '@', что указывает на чтение из файла
        if (input.size() > 0 && input[0] == L'@') {
            std::wifstream file(input.substr(1)); // Открываем файл после удаления символа '@'
            if (!file) {
                std::wcout << L"Ошибка при открытии файла." << std::endl;
                return 1;
            }

            while (std::getline(file, input)) {
                std::wistringstream iss(input);
                std::wstring param;

                while (iss >> param) {
                    if (param.empty()) {
                        continue;
                    }

                    if (param == L"-b") {
                        showInBytes = true;
                    }
                    else if (param == L"-a") {
                        showEachFile = true;
                    }
                    else if (param == L"-c") {
                        showTotalSize = true;
                    }
                    else if (param == L"-s") {
                        showOnlyTotalSize = true;
                    }
                    else {
                        fs::path path(param);
                        if (fs::exists(path)) {
                            if (fs::is_directory(path)) {
                                std::wcout << L"Указанный путь: " << path << std::endl;

                                if (showOnlyTotalSize) {
                                    uintmax_t totalSize = calculateTotalSize(path, showInBytes);
                                    std::wcout << L"Размер самой запрашиваемой директории " << path << L": ";
                                    if (showInBytes) {
                                        std::wcout << totalSize << L" байт\n";
                                    }
                                    else {
                                        std::wcout << totalSize << L" блоков по 512 байт\n";
                                    }
                                }
                                else {
                                    std::wcout << L"Список папок и их размеры в " << path << L":\n";
                                    for (const auto& entry : fs::recursive_directory_iterator(path)) {
                                        if (fs::is_directory(entry)) {
                                            uintmax_t folderSize = calculateTotalSize(entry, showInBytes);
                                            if (showInBytes) {
                                                std::wcout << L"Папка: " << entry.path().wstring() << L", Размер: " << folderSize << L" байт\n";
                                            }
                                            else {
                                                std::wcout << L"Папка: " << entry.path().wstring() << L", Размер: " << folderSize << L" блоков по 512 байт\n";
                                            }
                                        }
                                        else if (showEachFile) {
                                            uintmax_t fileSize = fs::file_size(entry);
                                            if (showInBytes) {
                                                std::wcout << L"Файл: " << entry.path().wstring() << L", Размер: " << fileSize << L" байт\n";
                                            }
                                            else {
                                                std::wcout << L"Файл: " << entry.path().wstring() << L", Размер: " << fileSize << L" блоков по 512 байт\n";
                                            }
                                        }
                                    }

                                    if (showTotalSize) {
                                        uintmax_t totalSize = calculateTotalSize(path, showInBytes);
                                        std::wcout << L"Общий размер " << path << L": ";
                                        if (showInBytes) {
                                            std::wcout << totalSize << L" байт\n";
                                        }
                                        else {
                                            std::wcout << totalSize << L" блоков по 512 байт\n";
                                        }
                                    }
                                }
                            }
                            else {
                                std::wcout << L"Указанный путь \"" << path << L"\" не является директорией.\n";
                            }
                        }
                        else {
                            std::wcout << L"Указанный путь \"" << path << L"\" не существует.\n";
                        }
                    }
                }
            }

            file.close(); // Закрываем файл после чтения всех строк
        }
        else {
            std::wistringstream iss(input);
            std::wstring param;

            while (iss >> param) {
                if (param.empty()) {
                    continue;
                }

                if (param == L"-b") {
                    showInBytes = true;
                }
                else if (param == L"-a") {
                    showEachFile = true;
                }
                else if (param == L"-c") {
                    showTotalSize = true;
                }
                else if (param == L"-s") {
                    showOnlyTotalSize = true;
                }
                else {
                    fs::path path(param);
                    if (fs::exists(path)) {
                        if (fs::is_directory(path)) {
                            std::wcout << L"Указанный путь: " << path << std::endl;

                            if (showOnlyTotalSize) {
                                uintmax_t totalSize = calculateTotalSize(path, showInBytes);
                                std::wcout << L"Размер самой запрашиваемой директории " << path << L": ";
                                if (showInBytes) {
                                    std::wcout << totalSize << L" байт\n";
                                }
                                else {
                                    std::wcout << totalSize << L" блоков по 512 байт\n";
                                }
                            }
                            else {
                                std::wcout << L"Список папок и их размеры в " << path << L":\n";
                                for (const auto& entry : fs::recursive_directory_iterator(path)) {
                                    if (fs::is_directory(entry)) {
                                        uintmax_t folderSize = calculateTotalSize(entry, showInBytes);
                                        if (showInBytes) {
                                            std::wcout << L"Папка: " << entry.path().wstring() << L", Размер: " << folderSize << "\n";
                                        }
                                        else {
                                            std::wcout << L"Папка: " << entry.path().wstring() << L", Размер: " << folderSize << "\n";
                                        }
                                    }
                                    else if (showEachFile) {
                                        uintmax_t fileSize = fs::file_size(entry);
                                        if (showInBytes) {
                                            std::wcout << L"Файл: " << entry.path().wstring() << L", Размер: " << fileSize << "\n";
                                        }
                                        else {
                                            std::wcout << L"Файл: " << entry.path().wstring() << L", Размер: " << fileSize << L" \n";
                                        }
                                    }
                                }

                                if (showTotalSize) {
                                    uintmax_t totalSize = calculateTotalSize(path, showInBytes);
                                    std::wcout << L"Общий размер " << path << L": ";
                                    if (showInBytes) {
                                        std::wcout << totalSize << L" байт\n";
                                    }
                                    else {
                                        std::wcout << totalSize << L" блоков по 512 байт\n";
                                    }
                                }
                            }
                        }
                        else {
                            std::wcout << L"Указанный путь \"" << path << L"\" не является директорией.\n";
                        }
                    }
                    else {
                        std::wcout << L"Указанный путь \"" << path << L"\" не существует.\n";
                    }
                }
            }
        }

        std::wcout << L"Введите данные для поиска, для поиска по файлу перед названием указать @имя файла(для завершения введите 'exit') : ";
        std::getline(std::wcin, input);
    }

    return 0;
}