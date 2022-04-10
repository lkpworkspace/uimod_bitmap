#include <cmath>
#include <memory>
#include <functional>
#include <unordered_map>

#include "SFEPanel.hpp"
#include "bpbase.pb.h"

typedef ::google::protobuf::Message pb_msg_t;
typedef std::shared_ptr<pb_msg_t> pb_msg_ptr_t;

static std::unordered_map<std::string, std::function<pb_msg_ptr_t()>> g_msg_map = {
    // {"bpmath.String", [](){ return std::make_shared<bp::String>(); }}
};

pb_msg_ptr_t create_msg(const std::string& msg_name) {
    if (g_msg_map.find(msg_name) == g_msg_map.end()) {
        return nullptr;
    }
    return g_msg_map[msg_name]();
}

pb_msg_ptr_t draw(std::vector<pb_msg_ptr_t> in_args) {
    auto res = std::make_shared<bp::Int>();
    res->set_var(0);
    if (in_args.size() != 3) {
        res->set_var(-1);
        return res;
    }
    auto width = std::dynamic_pointer_cast<::bp::Int>(in_args[0]);
    auto height = std::dynamic_pointer_cast<::bp::Int>(in_args[1]);
    auto pixels = std::dynamic_pointer_cast<::bp::UInt32Ary>(in_args[2]);
    Json::Value v;
    v["command"] = "draw";
    v["width"] = width->var();
    v["height"] = height->var();
    for (int i = 0; i < pixels->ary().size(); ++i) {
        v["pixels"].append(pixels->ary(i));
    }
    sfe::SFEPanel::SendMessage("uinode_draw", "bitmap", v);
    return res;
}

pb_msg_ptr_t example(void) {
    auto res = std::make_shared<bp::Int>();
    res->set_var(0);
    static int width = 10;
    static int height = 10;
    static std::vector<uint32_t> pixels = {
        0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0,
        0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff,
        0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0,
        0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff,
        0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0,
        0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff,
        0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0,
        0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff,
        0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0,
        0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff, 0x0, 0xffffffff,
    };
    static auto last_time = std::chrono::steady_clock::now();
    int swap_time_ms = 1000;
    auto now = std::chrono::steady_clock::now();
    auto cost_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_time).count();
    if (cost_ms < swap_time_ms) {
        return res;
    }
    last_time = now;
    Json::Value v;
    v["command"] = "draw";
    v["width"] = width;
    v["height"] = height;
    for (int i = 0; i < pixels.size(); ++i) {
        v["pixels"].append(pixels[i]);
    }
    sfe::SFEPanel::SendMessage("uinode_draw", "bitmap", v);
    for (int i = 0; i < pixels.size(); ++i) {
        pixels[i] = ~pixels[i];
    }
    return res;
}
