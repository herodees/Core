#pragma once
#include <string>
#include <algorithm>
#include <vector>

namespace box
{
    std::vector<std::string> open_file_dialog(std::string_view file, bool multiselect, std::string_view filter = "All|*.*|Text|*.TXT");
    std::string save_file_dialog(std::string_view file, std::string_view filter = "All|*.*|Text|*.TXT");
    void        open_default_application(std::string_view file);
    uint64_t    generate_uuid();
    enum class show_message_type
    {
        ok, ok_cancel, yes_no, yes_no_cancel, abort_retry_ignore, retry_cancel, abort_ignore
    };
    std::string_view show_message(std::string_view title, std::string_view text, show_message_type buttons);
} // namespace box