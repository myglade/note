#pragma once

#include "pch.h"
#include <string>

#include "json.hpp"

using json = nlohmann::json;

const std::string SERVER_URL = "http://localhost:5000/note/";

inline void MsgBox(const std::string& msg) {
    MessageBox(NULL, (LPCTSTR) msg.c_str(), "Error", MB_OK);
}